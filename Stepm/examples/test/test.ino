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
  sm.setms(250);while (sm.getms()>0){}
  sm.run(32, 3, true);              //start 128 steps forward (clockwise, right) 
  while (sm.where()>0){}             //wait until step motor arrives to its last destination
  sm.run(16, 3, false);              //start 64 steps backward (counterclockwise, left) 
}
