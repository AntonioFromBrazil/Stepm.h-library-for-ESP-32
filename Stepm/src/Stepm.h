#ifndef STEPM_H
#define STEPM_H

#include "Arduino.h"

class Stepm {

  public:

  Stepm(uint8_t pin4, uint8_t pin3, uint8_t pin2, uint8_t pin1);
  void begin();
  void run(uint32_t steps, uint8_t vel, boolean cw);
  uint32_t where();
  void setms(uint32_t ms);
  uint32_t getms();

  private:

  void onTimer1ms();
  void go();
  static Stepm *isrTable[];
  using isrFunct = void (*)();
  template<uint8_t NUM_INTERRUPTS = SOC_TIMER_GROUP_TOTAL_TIMERS>
  static isrFunct getIsr(uint8_t timerNumber);
  uint8_t p4,p3,p2,p1,inivel=5;
  volatile uint32_t xsteps=0,xms=0;
  volatile uint8_t xfase=0, xvel=5;
  volatile boolean xcw=true;


};

template<uint8_t NUM_INTERRUPTS>
Stepm::isrFunct Stepm::getIsr(uint8_t timerNumber) {
  if (timerNumber == (NUM_INTERRUPTS - 1)) {
    return [] {
      isrTable[NUM_INTERRUPTS - 1]->onTimer1ms();
    };
  }
  return getIsr < NUM_INTERRUPTS - 1 > (timerNumber);
}

template<>
inline Stepm::isrFunct Stepm::getIsr<0>(uint8_t timerNumber) {
  (void) timerNumber;
  return nullptr;
}

#endif

