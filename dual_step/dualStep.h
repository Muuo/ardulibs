/*
  dualStep.h - - Stepper library for Wiring/Arduino - Version 0.4
  
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

// ensure this library description is only included once
#ifndef dualStep_h
#define dualStep_h

// library interface description
class dualStep {
  public:
    // constructors:
    dualStep(int Ynumber_of_steps, int Xnumber_of_steps, int Ymotor_pin_1, int Ymotor_pin_2, int Xmotor_pin_1, int Xmotor_pin_2);
    dualStep(int Ynumber_of_steps, int Xnumber_of_steps, int Ymotor_pin_1, int Ymotor_pin_2, int Ymotor_pin_3, int Ymotor_pin_4, int Xmotor_pin_1, int Xmotor_pin_2, int Xmotor_pin_3, int Xmotor_pin_4);

    // speed setter method:
    void setYSpeed(long whatSpeed);
    void setXSpeed(long whatSpeed);

    // mover method:
    void step(int Ysteps, int Xsteps);

    int version(void);

  private:
    void stepMotor(int this_step, char motor);
    
    int Ydirection;        // Direction of rotation
    int Yspeed;          // Speed in RPMs
    int Xdirection;        // Direction of rotation
    int Xspeed;          // Speed in RPMs
    unsigned long Ystep_delay;    // delay between steps, in ms, based on speed
    unsigned long Xstep_delay;
    int Ynumber_of_steps;      // total number of steps this motor can take
    int Xnumber_of_steps;
    int pin_count;        // whether you're driving the motor with 2 or 4 pins
    int Ystep_number;        // which step the motor is on
    int Xstep_number;

    // motor pin numbers:
    int Ymotor_pin_1;
    int Ymotor_pin_2;
    int Ymotor_pin_3;
    int Ymotor_pin_4;

    int Xmotor_pin_1;
    int Xmotor_pin_2;
    int Xmotor_pin_3;
    int Xmotor_pin_4;
    
    long Ylast_step_time;      // time stamp in ms of when the last step was taken
    long Xlast_step_time;
};

#endif

