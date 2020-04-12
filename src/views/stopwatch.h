/* Base Stopwatch */
#include "../system.h"
#include "../hal.h"

class Stopwatch
{
  public:
    Stopwatch(System *System, HAL *Hal);
    void route();
  private:
    System *sys;
    HAL *hal;
    void renderStopwatchView(void);
    void drawDigit(int, int, int, uint16_t);
    bool stopWatchActive;
    uint16_t stopwatchTimer;
    uint16_t stopwatchMs;
    int curdig0;
    int curdig1;
    int curdig2;
    int curdig3;
    int prevdig0;
    int prevdig1;
    int prevdig2;
    int prevdig3;
};
