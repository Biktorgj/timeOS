/* Base clock */
#include "../system.h"
#include "../hal.h"

class WatchFace
{
  public:
    WatchFace(System *System, HAL *Hal);
    void route();
  private:
    System *sys;
    HAL *hal;
    void renderWatchFaceView(void);
    void renderDigitalWatchFace(DateTimeArray);
    void renderAnalogWatchFace(bool, DateTimeArray);
    void renderBattery(void);
    void renderSteps(void);
    void drawDigit(int, int, int, uint16_t);
    void renderDigit(int, int, int, int, uint16_t);
    DateTimeArray prevTime;
    int curPath;
};
