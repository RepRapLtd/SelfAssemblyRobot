//============================================================================
// Name        : GCodeGenerator.cpp
// Author      : Adrian Bowyer
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

double zDepth = 0.0;
double safeHeight = 1.0;
double rapidRate = 1000.0;
double feedRate = 100.0;
double plungeRate = 50.0;

void Prompt()
{
	cout << endl << "; Commands:" << endl;
	cout << ";  c: - circle" << endl;
	cout << ";  s: - square" << endl;
	cout << ";  r: - rectangle" << endl;
	cout << ";  o: - drill centre" << endl;
	cout << ";  h: - print this list" << endl;
	cout << ";  q: - quit" << endl<< endl;
}

void rectangle(double x, double y)
{
	cout << "G1 X" << x/2.0 << " Y" << y/2.0 << " F" << rapidRate << endl;
	cout << "G1 Z" << zDepth << " F" << plungeRate << endl;
	cout << "G1 X" << x/2.0 << " Y" << -y/2.0 << " F" << feedRate << endl;
	cout << "G1 X" << -x/2.0 << " Y" << -y/2.0 << " F" << feedRate << endl;
	cout << "G1 X" << -x/2.0 << " Y" << y/2.0 << " F" << feedRate << endl;
	cout << "G1 X" << x/2.0 << " Y" << y/2.0 << " F" << feedRate << endl;
	cout << "G1 Z" << safeHeight << " F" << rapidRate << endl;
}

void circle(double r)
{
	cout << "G1 X" << r << " Y0.0" << " F" << rapidRate << endl;
	cout << "G1 Z" << zDepth << " F" << plungeRate << endl;
	int polygon = (int)(0.5 + 30*r/10.0);
	if(polygon < 10)
		polygon = 10;
	double aInc = M_PI*2.0/(double)polygon;
	double a = 0.0;
	for(int p = 0; p < polygon; p++)
	{
		a += aInc;
		cout << "G1 X" << r*cos(a) << " Y" << r*sin(a) << " F" << feedRate << endl;
	}
	cout << "G1 Z" << safeHeight << " F" << rapidRate << endl;
}


void centre(double x, double y)
{
	cout << "G1 X" << x << " Y" << y << " F" << rapidRate << endl;
	cout << "G1 Z" << zDepth << " F" << plungeRate << endl;
	cout << "G1 Z" << safeHeight << " F" << rapidRate << endl;
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
			cout << "; Square side: ";
			cin >> r;
			rectangle(r, r);
			break;

		case 'o':
			cout << "; Centre X and Y: ";
			cin >> x >> y;
			centre(x, y);
			break;

		case 'r':
			cout << "; Rectangle X and Y: ";
			cin >> x >> y;
			rectangle(x, y);
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



