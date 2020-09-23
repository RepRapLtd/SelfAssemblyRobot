//============================================================================
// Name        : GCodeGenerator.cpp
// Author      : Adrian Bowyer
// Version     :
// Copyright   : GPL
// Description : Simple interactive program to generate G codes for geometric shapes
//============================================================================

/*
 * Tell it what shape you want and the dimensions and It'll output the appropriate G Codes to
 * make the shape.  You can repeat this to produce a whole G Code file.
 *
 * The input and output is preceded by ";" characters, so you can simply copy and paste
 * the entire session into a valid G Code file for execution.
 */

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

/*
 * Set these to whatever you want before you start.
 */
double zCutDepth = -0.3;
double safeZHeight = 1.0;
double rapidInAirFeedRate = 1000.0;
double xyCutFeedRate = 100.0;
double zCutPlungeRate = 50.0;

// These are used internally

double currentX = 0;
double currentY = 0;
double currentZ = 0;
bool liftZAtEndOfMove = true;

void Prompt()
{
	cout << endl << "; Commands:" << endl;
	cout << ";  c: - circle" << endl;
	cout << ";  s: - square" << endl;
	cout << ";  r: - rectangle" << endl;
	cout << ";  o: - drill centre" << endl;
	cout << ";  m: - move in air to a position" << endl;
	cout << ";  l: - cut a straight line to a position" << endl;
	cout << ";  u: - swap lifting at end of moves or not" << endl;
	cout << ";  h: - print this list" << endl;
	cout << ";  q: - quit" << endl << endl;
	cout << "; Z is ";
	if(!liftZAtEndOfMove)
		cout << "not ";
	cout << "being lifted at the end of each activity." << endl << endl;
}

void rectangle(double x, double y)
{
	cout << "G1 X" << currentX << " Y" << currentY << " F" << rapidInAirFeedRate << endl;
	currentZ = zCutDepth;
	cout << "G1 Z" << currentZ << " F" << zCutPlungeRate << endl;
	cout << "G1 X" << x << " Y" << currentY << " F" << xyCutFeedRate << endl;
	cout << "G1 X" << x << " Y" << y << " F" << xyCutFeedRate << endl;
	cout << "G1 X" << currentX << " Y" << y << " F" << xyCutFeedRate << endl;
	cout << "G1 X" << currentX << " Y" << currentY << " F" << xyCutFeedRate << endl;
	if(liftZAtEndOfMove)
	{
		currentZ = safeZHeight;
		cout << "G1 Z" << currentZ << " F" << rapidInAirFeedRate << endl;
	}
}

// Cut a circle centred at the current position
// Return the cutter to the centre in air when done

void circle(double r)
{
	currentZ = safeZHeight;
	cout << "G1 Z" << currentZ << " F" << rapidInAirFeedRate << endl;
	cout << "G1 X" << currentX + r << " Y" << currentY << " F" << rapidInAirFeedRate << endl;
	currentZ = zCutDepth;
	cout << "G1 Z" << currentZ << " F" << zCutPlungeRate << endl;
	int polygon = (int)(0.5 + 60*r/10.0);
	if(polygon < 20)
		polygon = 20;
	double aInc = M_PI*2.0/(double)polygon;
	double a = 0.0;
	for(int p = 0; p < polygon; p++)
	{
		a += aInc;
		double x = r*cos(a);
		double y = r*sin(a);
		cout << "G1 X" << currentX + x << " Y" << currentY + y << " F" << xyCutFeedRate << endl;
	}
	currentZ = safeZHeight;
	cout << "G1 Z" << currentZ << " F" << rapidInAirFeedRate << endl;
	cout << "G1 X" << currentX << " Y" << currentY << " F" << rapidInAirFeedRate << endl;
}

// Mark a centre spot.

void centre(double x, double y)
{
	currentX = x;
	currentY = y;
	cout << "G1 X" << currentX << " Y" << currentY << " F" << rapidInAirFeedRate << endl;
	currentZ = zCutDepth;
	cout << "G1 Z" << currentZ << " F" << zCutPlungeRate << endl;
	if(liftZAtEndOfMove)
	{
		currentZ = safeZHeight;
		cout << "G1 Z" << currentZ << " F" << rapidInAirFeedRate << endl;
	}
}

// Move in air to a position

void move(double x, double y)
{
	currentZ = safeZHeight;
	cout << "G1 Z" << currentZ << " F" << rapidInAirFeedRate << endl;
	currentX = x;
	currentY = y;
	cout << "G1 X" << currentX << " Y" << currentY << " F" << rapidInAirFeedRate << endl;
}

// Cut a straight line

void cut(double x, double y)
{
	currentZ = zCutDepth;
	cout << "G1 Z" << currentZ << " F" << zCutPlungeRate << endl;
	currentX = x;
	currentY = y;
	cout << "G1 X" << currentX << " Y" << currentY << " F" << xyCutFeedRate << endl;
	if(liftZAtEndOfMove)
	{
		currentZ = safeZHeight;
		cout << "G1 Z" << currentZ << " F" << rapidInAirFeedRate << endl;
	}
}

void Control()
{
	double r, x, y;


	cout << "; Type h for help." << endl;
	while(1)
	{
		cout << "; Command: ";
		char c;
		cin >> c;

		switch(c)
		{
		case 'c':
			cout << "; Circle radius: ";
			cin >> r;
			circle(r);
			break;

		case 's':
			cout << "; Square side (bottom left will be the current position): ";
			cin >> r;
			rectangle(r, r);
			break;

		case 'o':
			cout << "; Centre X and Y: ";
			cin >> x >> y;
			centre(x, y);
			break;

		case 'm':
			cout << "; X and Y to move to: ";
			cin >> x >> y;
			move(x, y);
			break;

		case 'l':
			cout << "; X and Y to cut to: ";
			cin >> x >> y;
			cut(x, y);
			break;

		case 'r':
			cout << "; Rectangle X and Y (bottom left will be the current position): ";
			cin >> x >> y;
			rectangle(x, y);
			break;

		case 'u':
			liftZAtEndOfMove = !liftZAtEndOfMove;
			cout << "; Z will ";
			if(!liftZAtEndOfMove)
				cout << "not ";
			cout << "be lifted at the end of each activity." << endl;
			break;


		case 'q':
			return;

		default:
			cout << endl << "; Unrecognised command - " << c << endl;
		case 'h':
			Prompt();
		}
	}
}



int main()
{

	cout << std::fixed;
	cout << std::setprecision(2);

	cout << "; G Code Generator" << endl;

	Control();

	cout << "; Bye!" << endl;

	return 0;
}



