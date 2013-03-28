/*	AControl.c
	This library will provide useful functions for translation along the x and y axes
	along with functions for linear and circular interpolation.

	Hopefully I'll also include functions that allow for callibration of the device.
*/

#include Arduino.h
#include AControl.h
#include dualStep.h

//initialize the motor speeds
Acontrol::init(int maxspeed) {
	this->maxspeed = maxspeed;
	drives.setXSpeed(maxspeed);
	drives.setYSpeed(maxspeed);
}

AControl::gox(int len) {
	int steps = len //this will be changed to some transformation involving len... will be determined after callibration
	drives.step(steps,0);
}

AControl::goy(int len) {
	int steps = len //this will be changed to some transformation involving len... will be determined after callibration
	drives.step(0,steps);
}