------------------------------------------------------------------------------
This is a nonblock step motor (28byj-48) library based on interrupts
By Antonio Testa, March/2024

Special thanks to @gfvalvo from https://forum.arduino.cc who fixed the error: invalid-use-of-non-static-member-function and gave all tricks to use hardware timers inside the library

https://forum.arduino.cc/t/error-invalid-use-of-non-static-member-function/1240240

------------------------------------------------------------------------------
In the main program is mandatory to define the following:

#include <Stepm.h>     //include the NewStep.h elibrary                   

Stepm name(5,17,16,4); //define instance for ULN2003 driver motor pins in1,in2,in3,in4

name.begin(mode);      //At setup(), issue begin() command to init the library timer and set stepmotor mode operation
------------------------------------------------------------------------------

CONSTRUCTOR:

Stepm name(pin4, pin3, pin2, pin1); 
name is any name for instance stepmotor
uint8_t pin4,3,2,1 reffers to ESP-32 pins connected to in4,3,2,1 of ULN2003 motor driver


METHODS:

name.begin(uint8_t mode);
at setup() section, to init the library timer and step motor mode operation
uint8_t mode should be:
0=full step, low torque, low consumption
1=full step, high torque, high consumption
2=half step, middle torque, middle consumption  

name.run(steps,vel,cw);
uint32_t steps is the number of steps to travel (512 steps per turn)
uint8_t vel is the speed (max 3 for right direction, 5 for left direction) increase value means decrease speed
boolean cw define direction of turn (true for right, clockwise; false for left, counterclockwise)
IMPORTANT: the name.run can only be called if the name.where()==0  

name.where();
retun in an uint32_t value the number of steps remainding to motor arrives to destination
if name.where()==0 means the motor is already in the last destination and is stopped at this moment

name.setms(uint32_t ms);
set the ms counter down (nonblock to be used in substitution of delay function).
As soon as initiated this ms counter is decremented each millisecond

name.getms();
return the ms counter at moment (initial ms value setted by previously name.setms)
------------------------------------------------------------------------------

//
// library Stepm.h
// Simple example of use
// move 4096 steps forward (clockwise, right) following by 4096 steps backward (counterclockwise, left)
// 
// function used:
// begin() - init the timer library
// where() - returns the number of steps remainding to stepmotor gets to it's destination
// run(steps,speed,direction) - cause step motor to move according to parameters informed
// 

#include <Stepm.h>

Stepm sm(5,17,16,4);                   //define constructor for 28byj-48 and ULN2003 driver motor pins in1,in2,in3,in4

void setup()
{
  sm.begin(2);                         //define the step motor mode operation (always 4096 steps per turn) 
                                       //0=full step, low torque, low consumption
                                       //1=full step, high torque, high consumption
                                       //2=half step, middle torque, middle consumption

                                       //the max speed depending on mode selected:
                                       //mode 0 - max speed 3 for clockwise and 5 for counterclockwise
                                       //mode 1 - max speed 3 for clockwise and 5 for counterclockwise
                                       //mode 2 - max speed 2 for clockwise and 2 for counterclockwise
}

void loop()
{
  while (sm.where()>0){}               //wait until step motor arrives to its last destination
  sm.setms(500);while (sm.getms()>0){} //wait 500ms
  sm.run(4096, 2, true);               //start 4096 steps forward (clockwise, right) 
  while (sm.where()>0){}               //wait until step motor arrives to its last destination
  sm.setms(500);while (sm.getms()>0){} //wait 500ms
  sm.run(4096, 2, false);              //start 4096 steps backward (counterclockwise, left) 
}

------------------------------------------------------------------------------
