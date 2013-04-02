/*	AControl.c
	This library will provide useful functions for translation along the x and y axes
	along with functions for linear and circular interpolation.

	Hopefully I'll also include functions that allow for callibration of the device.
*/

/* to do: 	-implement cartesian co-ordinate system
			-implement setpos(int x, int y) that takes the printer head to a position (x,y)
			-implement circular interpolation												 */

#include Arduino.h
#include AControl.h
#include dualStep.h
#include <math.h>

//initialize the motor speeds
Acontrol::init(int maxspeed) {
	this->maxspeed = maxspeed;
	drives.setXSpeed(maxspeed);
	drives.setYSpeed(maxspeed);
}

AControl::gox(int len) {
	int steps = len; //this will be changed to some transformation involving len... will be determined after callibration
	drives.step(0,steps);
	XPos+=len;
}

AControl::goy(int len) {
	int steps = len; //this will be changed to some transformation involving len... will be determined after callibration
	drives.step(steps,0);
	YPos+=len;
}

Acontrol::intpol(int x1, int y1, int x2, int y2) {
	//determine change in x assume that (x1,y1) is the origin and (x2,y2) the destination
	int dx = x1-x2;
	//do the same thing for y
	int dy = y1-y2;
	int Xsteps = dx; //this will be changed to some transformation involving dx
	int Ysteps = dy; //this will be changed to some transformation involving dy

	if (abs(dx)>abs(dy)) {
		drives.setXSpeed(maxspeed);
		drives.setYSpeed((maxspeed*dy)/dx);
	}
	else if (abs(dy)>abs(dx)) {
		drives.setXSpeed((maxspeed*dx)/dy);
		drives.setYSpeed(maxspeed);
	}

	drives.step(Ysteps, Xsteps);
	XPos+=dx;
	YPos+=dy;
}

Acontrol::intpolar(int x, int y, int radius, int theta) {
	//determine change in x... (x,y) is the origin, theta is the angle of the line relative to the x-axis
	//radius is the length of the projected line
	/*              ^
				   Y|
					|  
					| 
					|
					|-------------->
								   X			*/ 
	int dx = radius * cos(theta);
	int dy = radius * sin(theta);
	int Xsteps = dx; //this will be changed to some transformation involving dx
	int Ysteps = dy; //this will be changed to some transformation involving dy

	if (abs(dx)>abs(dy)) {
		drives.setXSpeed(maxspeed);
		drives.setYSpeed((maxspeed*dy)/dx);
	}
	else if (abs(dy)>abs(dx)) {
		drives.setXSpeed((maxspeed*dx)/dy);
		drives.setYSpeed(maxspeed);
	}

	drives.step(Ysteps, Xsteps);
	XPos+=dx;
	YPos+=dy;
}

AControl::circpol(int radius, int a, int b) {

}