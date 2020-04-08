/* Battery and charger */
#define BATT_VCC 31 // input
#define BATT_CHG 12 // input

class PowerMGR
{
  public:
    PowerMGR();
    float getBatteryVoltage();
    unsigned int getBatteryPercentage();
    bool isCharging();
    bool gotoSleep();
  private:
    float _currentVoltage;
    unsigned int _currentPercentage;
};
