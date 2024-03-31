This is a nonblock step motor (28byj-48) library based on interrupts
By Antonio Testa, March/2024

------------------------------------------------------------------------------
In the main program is mandatory to define the following:

#include <Stepm.h>     //include the NewStep.h elibrary                   

Stepm name(4,16,17,5); //define instance for ULN2003 driver motor pins in4,in3,in2,in1

name.begin();          //At setup() section, issue this begin() command to init the library timer

------------------------------------------------------------------------------

define instance:

Stepm name(pin4, pin3, pin2, pin1) 
name is any name for instance stepmotor
uint8_t pin4,3,2,1 reffers to ESP-32 pins connected to in4,3,2,1 of ULN2003 motor driver


methods:

name.begin();
at setup() section, to init the library timer  

name.run(steps,vel,cw);
uint32_t steps is the number of steps to travel (512 steps per turn)
uint8_t vel is the speed (máx 3 for right direction, 5 for left direction) increase value means decrease speed
boolean cw define direction of turn (true for right, clockwise; false for left, counterclockwise)
IMPORTANT: the name.run can only be called if the name.where()==0  

name.where();
retun in an uint32_t value the number of steps remainding to motor arrives to destination
if name.where()==0 means the motor is already in the last destination and is stopped at this moment

name.setms(uint32_t ms);
set the ms counter down (nonblock to be used in substitution of delay function).
As soon as initiated this ms counter is decremented each millisecond

name.getms();
return the ms counter at moment (initial value setted by previously name.setms)

------------------------------------------------------------------------------

//
// library Stepm.h
// Simple example of use
// move 128 steps forward (clockwise, right) following by 64 steps backward (counterclockwise, left)
// 
// function used:
// begin() - init the timer library
// where() - returns the number of steps remainding to stepmotor gets to it's destination
// run(steps,speed,direction) - cause step motor to move according to parameters informed
// 

#include <Stepm.h>

Stepm sm(4,16,17,5);                 //define instance for ULN2003 driver motor pins in4,in3,in2,in1

void setup()
{
  sm.begin();
}

void loop()
{
  while (sm.where()>0){}             //wait until step motor arrives to its last destination
  sm.run(128, 3, true);              //start 128 steps forward (clockwise, right) 
  while (sm.where()>0){}             //wait until step motor arrives to its last destination
  sm.run(64, 3, false);              //start 64 steps backward (counterclockwise, left) 
}

------------------------------------------------------------------------------
