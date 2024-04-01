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

Stepm sm(5,17,16,4);                   //define constructor for ULN2003 driver motor pins in1,in2,in3,in4

void setup()
{
  sm.begin(2);                         //define the step motor mode operation (always 4096 steps per turn) 
                                       //0=full step, low torque, low consumption
                                       //1=full step, high torque, high consumption
                                       //2=half step, middle torque, middle consumption

                                       //the max speed depending on mode selected:
                                       //mode 0 - max speed 3 for clockwise and 5 for counterclockwise
                                       //mode 1 - max speed 3 for clockwise and 5 for counterclockwise
                                       //mode 2 - max speed 1 for clockwise and 2 for counterclockwise
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
