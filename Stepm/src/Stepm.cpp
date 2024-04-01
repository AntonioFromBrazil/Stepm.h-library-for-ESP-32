#include "Stepm.h"

//----------------------------------------------------------------------
Stepm::Stepm(uint8_t pin4, uint8_t pin3, uint8_t pin2, uint8_t pin1)
{
  pinMode(pin4,OUTPUT);                              //define the driver motor pins as output
  pinMode(pin3,OUTPUT);                              //
  pinMode(pin2,OUTPUT);                              //
  pinMode(pin1,OUTPUT);                              //

  digitalWrite(pin4,0);                              //initialize then with zeroes
  digitalWrite(pin3,0);                              //
  digitalWrite(pin2,0);                              //
  digitalWrite(pin1,0);                              //

  p4=pin4, p3=pin3, p2=pin2, p1=pin1;                //copy pins to internal variables
}


//----------------------------------------------------------------------
void Stepm::begin(uint8_t mode) {
  xmode=mode;
  const uint8_t timerNumber = 0;
  hw_timer_t *timer1ms = NULL;
  timer1ms = timerBegin(timerNumber, 80, true);
  isrTable[timerNumber] = this;
  auto isr = getIsr(timerNumber);
  timerAttachInterrupt(timer1ms, isr, false);
  timerAlarmWrite(timer1ms, 1000, true);
  timerAlarmEnable(timer1ms);
}


//----------------------------------------------------------------------
void Stepm::run(uint32_t steps, uint8_t vel, boolean cw)
{
  inivel=vel;xvel=vel;xcw=cw;xsteps=steps/8;
}


//----------------------------------------------------------------------
uint32_t Stepm::where()
{
  return xsteps;
}


//----------------------------------------------------------------------
void Stepm::setms(uint32_t ms){
  xms=ms;
}


//----------------------------------------------------------------------
uint32_t Stepm::getms() {
  return xms;
}


//----------------------------------------------------------------------
void IRAM_ATTR Stepm::onTimer1ms()
{
  if (xms>0){xms--;}
  xvel--;
  if (xvel==0){
    xvel=inivel;
    if (xsteps>0){
      Stepm::go();
      xfase++;
      if ((xfase==4)&&(xmode<2)){xfase=0;xsteps--;}
      if ((xfase==8)&&(xmode>1)){xfase=0;xsteps--;}
    }
    if (xsteps==0){digitalWrite(p1, 0);digitalWrite(p2, 0);digitalWrite(p3, 0);digitalWrite(p4, 0);}
  }
}

Stepm *Stepm::isrTable[SOC_TIMER_GROUP_TOTAL_TIMERS];


//----------------------------------------------------------------------
void Stepm::go()
{

  if (xmode==0){
    if (xcw){
      if (xfase==0){Stepm::writ(0,0,0,1);} //0x01
      if (xfase==1){Stepm::writ(0,0,1,0);} //0x02
      if (xfase==2){Stepm::writ(0,1,0,0);} //0x04
      if (xfase==3){Stepm::writ(1,0,0,0);} //0x08
    }
    if (!xcw){
      if (xfase==0){Stepm::writ(1,0,0,0);} //0x08
      if (xfase==1){Stepm::writ(0,1,0,0);} //0x04
      if (xfase==2){Stepm::writ(0,0,1,0);} //0x02
      if (xfase==3){Stepm::writ(0,0,0,1);} //0x01
    }
  }


  if (xmode==1){
    if (xcw){
      if (xfase==0){Stepm::writ(1,0,0,1);} //0x09
      if (xfase==1){Stepm::writ(0,0,1,1);} //0x03
      if (xfase==2){Stepm::writ(0,1,1,0);} //0x06
      if (xfase==3){Stepm::writ(1,1,0,0);} //0x0C    
    }
    if (!xcw){
      if (xfase==0){Stepm::writ(1,1,0,0);} //0x0C 
      if (xfase==1){Stepm::writ(0,1,1,0);} //0x06
      if (xfase==2){Stepm::writ(0,0,1,1);} //0x03
      if (xfase==3){Stepm::writ(1,0,0,1);} //0x09    
    }
  }


  if (xmode==2){
    if (!xcw){
      if (xfase==0){Stepm::writ(1,0,0,0);} //0x08
      if (xfase==1){Stepm::writ(1,1,0,0);} //0x0C 
      if (xfase==2){Stepm::writ(0,1,0,0);} //0x04
      if (xfase==3){Stepm::writ(0,1,1,0);} //0x06
      if (xfase==4){Stepm::writ(0,0,1,0);} //0x02
      if (xfase==5){Stepm::writ(0,0,1,1);} //0x03
      if (xfase==6){Stepm::writ(0,0,0,1);} //0x01
      if (xfase==7){Stepm::writ(1,0,0,1);} //0x09
    }

    if (xcw){
      if (xfase==0){Stepm::writ(1,0,0,1);} //0x09
      if (xfase==1){Stepm::writ(0,0,0,1);} //0x01
      if (xfase==2){Stepm::writ(0,0,1,1);} //0x03
      if (xfase==3){Stepm::writ(0,0,1,0);} //0x02    
      if (xfase==4){Stepm::writ(0,1,1,0);} //0x06
      if (xfase==5){Stepm::writ(0,1,0,0);} //0x04
      if (xfase==6){Stepm::writ(1,1,0,0);} //0x0C
      if (xfase==7){Stepm::writ(1,0,0,0);} //0x08 
    }
  }
}

//----------------------------------------------------------------------
void Stepm::writ(uint8_t px1, uint8_t px2, uint8_t px3, uint8_t px4)
{
  digitalWrite(p1, px1);
  digitalWrite(p2, px2);
  digitalWrite(p3, px3);
  digitalWrite(p4, px4);
}

//----------------------------------------------------------------------
