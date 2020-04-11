#ifndef _HRM_
#define _HRM_
#include "HRS3300lib.h"

class HeartRateMonitor
{
  public:
    HeartRateMonitor();
    void enable();
    void disable();
    uint8_t getHR();
  private:
    HRS3300lib HRS3300;
};




#endif
