/* Base clock */
#include "../system.h"
#include "../hal.h"

class Clock
{
  public:
    Clock(System *System, HAL *Hal);
    void route();
  private:
    System *sys;
    HAL *hal;
    void renderClockView(void);
    void renderDigitalClock(DateTimeArray);
    void renderAnalogClock(bool, DateTimeArray);
    void renderBattery(void);
    void renderSteps(void);
    DateTimeArray prevTime;
    int curPath;
};
