/*
  dualStep.cpp - - Stepper library for Wiring/Arduino - Version 0.4
  
  This is a modification of stepper.cpp (the details of which are below) to allow for the simultaneous control of 2 stepper motors
  
  Original library     (0.1) by Tom Igoe.
  Two-wire modifications   (0.2) by Sebastian Gassner
  Combination version   (0.3) by Tom Igoe and David Mellis
  Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley  

  Drives a unipolar or bipolar stepper motor using  2 wires or 4 wires

  When wiring multiple stepper motors to a microcontroller,
  you quickly run out of output pins, with each motor requiring 4 connections. 

  By making use of the fact that at any time two of the four motor
  coils are the inverse  of the other two, the number of
  control connections can be reduced from 4 to 2. 

  A slightly modified circuit around a Darlington transistor array or an L293 H-bridge
  connects to only 2 microcontroler pins, inverts the signals received,
  and delivers the 4 (2 plus 2 inverted ones) output signals required
  for driving a stepper motor.

  The sequence of control signals for 4 control wires is as follows:

  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1

  The sequence of controls signals for 2 control wires is as follows
  (columns C1 and C2 from above):

  Step C0 C1
     1  0  1
     2  1  1
     3  1  0
     4  0  0

  The circuits can be found at 
 
http://www.arduino.cc/en/Tutorial/Stepper
 
 
 */


#include "Arduino.h"
#include "dualStep.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
dualStep::dualStep(int Ynumber_of_steps, int Xnumber_of_steps, int Ymotor_pin_1, int Ymotor_pin_2, int Xmotor_pin_1, int Xmotor_pin_2)
{
  this->Ystep_number = 0;      // which step the motor is on
  this->Yspeed = 0;        // the motor speed, in revolutions per minute
  this->Ydirection = 0;      // motor direction
  this->Ylast_step_time = 0;    // time stamp in ms of the last step taken
  this->Ynumber_of_steps = Ynumber_of_steps;    // total number of steps for this motor

  this->Xstep_number = 0;      // which step the motor is on
  this->Xspeed = 0;        // the motor speed, in revolutions per minute
  this->Xdirection = 0;      // motor direction
  this->Xlast_step_time = 0;    // time stamp in ms of the last step taken
  this->Xnumber_of_steps = Xnumber_of_steps;    // total number of steps for this motor
  
  // Arduino pins for the motor control connection:
  this->Ymotor_pin_1 = Ymotor_pin_1;
  this->Ymotor_pin_2 = Ymotor_pin_2;

  this->Xmotor_pin_1 = Xmotor_pin_1;
  this->Xmotor_pin_2 = Xmotor_pin_2;

  // setup the pins on the microcontroller:
  pinMode(this->Ymotor_pin_1, OUTPUT);
  pinMode(this->Ymotor_pin_2, OUTPUT);

  pinMode(this->Xmotor_pin_1, OUTPUT);
  pinMode(this->Xmotor_pin_2, OUTPUT);
  
  // When there are only 2 pins, set the other two to 0:
  this->Ymotor_pin_3 = 0;
  this->Ymotor_pin_4 = 0;

  this->Xmotor_pin_3 = 0;
  this->Xmotor_pin_4 = 0;
  
  // pin_count is used by the stepMotor() method:
  this->pin_count = 2;
}


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

dualStep::dualStep(int Ynumber_of_steps, int Xnumber_of_steps, int Ymotor_pin_1, int Ymotor_pin_2, int Ymotor_pin_3, int Ymotor_pin_4, int Xmotor_pin_1, int Xmotor_pin_2, int Xmotor_pin_3, int Xmotor_pin_4)
{
  this->Ystep_number = 0;      // which step the motor is on
  this->Yspeed = 0;        // the motor speed, in revolutions per minute
  this->Ydirection = 0;      // motor direction
  this->Ylast_step_time = 0;    // time stamp in ms of the last step taken
  this->Ynumber_of_steps = Ynumber_of_steps;    // total number of steps for this motor

  this->Xstep_number = 0;      // which step the motor is on
  this->Xspeed = 0;        // the motor speed, in revolutions per minute
  this->Xdirection = 0;      // motor direction
  this->Xlast_step_time = 0;    // time stamp in ms of the last step taken
  this->Xnumber_of_steps = Xnumber_of_steps;    // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->Ymotor_pin_1 = Ymotor_pin_1;
  this->Ymotor_pin_2 = Ymotor_pin_2;
  this->Ymotor_pin_3 = Ymotor_pin_3;
  this->Ymotor_pin_4 = Ymotor_pin_4;

  this->Xmotor_pin_1 = Xmotor_pin_1;
  this->Xmotor_pin_2 = Xmotor_pin_2;
  this->Xmotor_pin_3 = Xmotor_pin_3;
  this->Xmotor_pin_4 = Xmotor_pin_4;

  // setup the pins on the microcontroller:
  pinMode(this->Ymotor_pin_1, OUTPUT);
  pinMode(this->Ymotor_pin_2, OUTPUT);
  pinMode(this->Ymotor_pin_3, OUTPUT);
  pinMode(this->Ymotor_pin_4, OUTPUT);

  pinMode(this->Xmotor_pin_1, OUTPUT);
  pinMode(this->Xmotor_pin_2, OUTPUT);
  pinMode(this->Xmotor_pin_3, OUTPUT);
  pinMode(this->Xmotor_pin_4, OUTPUT);

  // pin_count is used by the stepMotor() method:  
  this->pin_count = 4;  
}

/*
  Sets the speed in revs per minute

*/
void dualStep::setYSpeed(long whatSpeed)
{
  this->Ystep_delay = 60L * 1000L / this->Ynumber_of_steps / whatSpeed;
}

void dualStep::setXSpeed(long whatSpeed)
{
  this->Xstep_delay = 60L * 1000L / this->Xnumber_of_steps / whatSpeed;
}

/*
  Moves the motor steps_to_move steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
void dualStep::step(int Ysteps=0, int Xsteps=0)
{ 
  int Ysteps_left = abs(Ysteps);  // how many steps to take 
  int Xsteps_left = abs(Xsteps);  // how many steps to take
  
  // determine direction based on whether steps_to_mode is + or -:
  if (Ysteps > 0) {this->Ydirection = 1;}
  if (Ysteps < 0) {this->Ydirection = 0;}

  if (Xsteps > 0) {this->Xdirection = 1;}
  if (Xsteps < 0) {this->Xdirection = 0;}
  
    
  // decrement the number of steps, moving one step each time:
  while(Xsteps_left > 0 || Ysteps_left > 0) {
    
    //block for the Ymotor
    if(Ysteps_left > 0) {
      if (millis() - this->Ylast_step_time >= this->Ystep_delay) {
        // move only if the appropriate delay has passed:
        // get the timeStamp of when you stepped:
        this->Ylast_step_time = millis();
        // increment or decrement the step number,
        // depending on direction:
        if (this->Ydirection == 1) {
          this->Ystep_number++;
          if (this->Ystep_number == this->Ynumber_of_steps) {
            this->Ystep_number = 0;
          }
        } 
        else { 
          if (this->Ystep_number == 0) {
            this->Ystep_number = this->Ynumber_of_steps;
          }
          this->Ystep_number--;
        }
        // decrement the steps left:
        Ysteps_left--;
        // step the motor to step number 0, 1, 2, or 3:
        stepMotor(this->Ystep_number % 4, 0);
      }
    }

    //block for the Xmotor
    if(Xsteps_left > 0) {
      if (millis() - this->Xlast_step_time >= this->Xstep_delay) {
        // move only if the appropriate delay has passed:
        // get the timeStamp of when you stepped:
        this->Xlast_step_time = millis();
        // increment or decrement the step number,
        // depending on direction:
        if (this->Xdirection == 1) {
          this->Xstep_number++;
          if (this->Xstep_number == this->Xnumber_of_steps) {
            this->Xstep_number = 0;
          }
        } 
        else { 
          if (this->Xstep_number == 0) {
            this->Xstep_number = this->Xnumber_of_steps;
          }
          this->Xstep_number--;
        }
        // decrement the steps left:
        Xsteps_left--;
        // step the motor to step number 0, 1, 2, or 3:
        stepMotor(this->Xstep_number % 4, 1);
      }
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void dualStep::stepMotor(int thisStep, char motor)
{
  if (motor==0) {
    if (this->pin_count == 2) {
      switch (thisStep) {
        case 0: /* 01 */
        digitalWrite(Ymotor_pin_1, LOW);
        digitalWrite(Ymotor_pin_2, HIGH);
        break;
        case 1: /* 11 */
        digitalWrite(Ymotor_pin_1, HIGH);
        digitalWrite(Ymotor_pin_2, HIGH);
        break;
        case 2: /* 10 */
        digitalWrite(Ymotor_pin_1, HIGH);
        digitalWrite(Ymotor_pin_2, LOW);
        break;
        case 3: /* 00 */
        digitalWrite(Ymotor_pin_1, LOW);
        digitalWrite(Ymotor_pin_2, LOW);
        break;
      } 
    }
    if (this->pin_count == 4) {
      switch (thisStep) {
        case 0:    // 1010
        digitalWrite(Ymotor_pin_1, HIGH);
        digitalWrite(Ymotor_pin_2, LOW);
        digitalWrite(Ymotor_pin_3, HIGH);
        digitalWrite(Ymotor_pin_4, LOW);
        break;
        case 1:    // 0110
        digitalWrite(Ymotor_pin_1, LOW);
        digitalWrite(Ymotor_pin_2, HIGH);
        digitalWrite(Ymotor_pin_3, HIGH);
        digitalWrite(Ymotor_pin_4, LOW);
        break;
        case 2:    //0101
        digitalWrite(Ymotor_pin_1, LOW);
        digitalWrite(Ymotor_pin_2, HIGH);
        digitalWrite(Ymotor_pin_3, LOW);
        digitalWrite(Ymotor_pin_4, HIGH);
        break;
        case 3:    //1001
        digitalWrite(Ymotor_pin_1, HIGH);
        digitalWrite(Ymotor_pin_2, LOW);
        digitalWrite(Ymotor_pin_3, LOW);
        digitalWrite(Ymotor_pin_4, HIGH);
        break;
      } 
    }
  }
  else {
    if (this->pin_count == 2) {
      switch (thisStep) {
        case 0: /* 01 */
        digitalWrite(Xmotor_pin_1, LOW);
        digitalWrite(Xmotor_pin_2, HIGH);
        break;
        case 1: /* 11 */
        digitalWrite(Xmotor_pin_1, HIGH);
        digitalWrite(Xmotor_pin_2, HIGH);
        break;
        case 2: /* 10 */
        digitalWrite(Xmotor_pin_1, HIGH);
        digitalWrite(Xmotor_pin_2, LOW);
        break;
        case 3: /* 00 */
        digitalWrite(Xmotor_pin_1, LOW);
        digitalWrite(Xmotor_pin_2, LOW);
        break;
      } 
    }
    if (this->pin_count == 4) {
        switch (thisStep) {
          case 0:    // 1010
          digitalWrite(Xmotor_pin_1, HIGH);
          digitalWrite(Xmotor_pin_2, LOW);
          digitalWrite(Xmotor_pin_3, HIGH);
          digitalWrite(Xmotor_pin_4, LOW);
          break;
          case 1:    // 0110
          digitalWrite(Xmotor_pin_1, LOW);
          digitalWrite(Xmotor_pin_2, HIGH);
          digitalWrite(Xmotor_pin_3, HIGH);
          digitalWrite(Xmotor_pin_4, LOW);
          break;
          case 2:    //0101
          digitalWrite(Xmotor_pin_1, LOW);
          digitalWrite(Xmotor_pin_2, HIGH);
          digitalWrite(Xmotor_pin_3, LOW);
          digitalWrite(Xmotor_pin_4, HIGH);
          break;
          case 3:    //1001
          digitalWrite(Xmotor_pin_1, HIGH);
          digitalWrite(Xmotor_pin_2, LOW);
          digitalWrite(Xmotor_pin_3, LOW);
          digitalWrite(Xmotor_pin_4, HIGH);
          break;
        } 
    }
  }
}

/*
  version() returns the version of the library:
*/
int dualStep::version(void)
{
  return 4;
}
