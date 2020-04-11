#ifndef _POWER_
#define _POWER_

class Power
{
  public:
    Power();
    float getBatteryVoltage();
    unsigned int getBatteryPercentage();
    bool isCharging();
    void suspend();
  private:
    float _currentVoltage;
    unsigned int _currentPercentage;
};

#endif
