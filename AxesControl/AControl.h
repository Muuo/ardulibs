/*	AControl.h
	This library will provide useful functions for translation along the x and y axes
	along with functions for linear and circular interpolation.

	Hopefully I'll also include functions that allow for callibration of the device.
*/

#include dualStep.h

//ensure the library is only included once
#ifndef AControl_h
#define AControl_h

class AControl.h {
	public:
		//contructor
		AControl(int Xnumber_of_steps, int Ynumber_of_steps, int Xmotor_pin_1, int Xmotor_pin_2, int Xmotor_pin_3, int Xmotor_pin_4, int Ymotor_pin_1, int Ymotor_pin_2, int Ymotor_pin_3, int Ymotor_pin_4) : drives (Ynumber_of_steps, Xnumber_of_steps, Ymotor_pin_1, Ymotor_pin_2, Ymotor_pin_3, Ymotor_pin_4, Xmotor_pin_1, Xmotor_pin_2, Xmotor_pin_3, Xmotor_pin_4); {XPos=0;YPos=0;}
		AControl(int Xnumber_of_steps, int Ynumber_of_steps, int Xmotor_pin_1, int Xmotor_pin_2, int Ymotor_pin_1, int Ymotor_pin_2) : drives (Ynumber_of_steps, Xnumber_of_steps, Ymotor_pin_1, Ymotor_pin_2, Xmotor_pin_1, Xmotor_pin_2); {XPos=0;YPos=0;}

		//an initializer... sets the speeds and everything
		void init(int maxspeed);

		//CONTROL
		//**linear transformations**//
		void gox(int len);
		void goy(int len);
		void intpol(int x1, int y1, int x2, int y2);
		void intpolar(int x, int y, int radius, int theta);
		void circpol(int radius, int a, int b);

	private:
		dualStep drives;
		int maxspeed;
		int XPos;
		int YPos;
};

#endif