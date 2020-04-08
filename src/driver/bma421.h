#include "Arduino.h"
#include <Wire.h>

#define BMA400_INTERRUPT 8

#define BMA400_CHIP_ID			0x00
// Definitions as specified in Bosch's sample code
/**\name	ENABLE/DISABLE BIT VALUES    */
#define	BMA4_ENABLE			UINT8_C(0x01)
#define	BMA4_DISABLE		UINT8_C(0x00)
#define	BMA4_CONFIG_STREAM_SIZE		UINT16_C(6144)
#define	BMA4_INIT_CTRL_ADDR			UINT8_C(0x59)
#define	BMA4_INTERNAL_STAT			UINT8_C(0x2A)
#define	BMA4_ASIC_INITIALIZED		UINT8_C(0x01)
#define BMA423_FEATURE_SIZE			UINT8_C(64)

/**\name FEATURE CONFIG RELATED */
#define BMA4_RESERVED_REG_5B_ADDR		UINT8_C(0x5B)
#define BMA4_RESERVED_REG_5C_ADDR		UINT8_C(0x5C)
#define	BMA4_FEATURE_CONFIG_ADDR		UINT8_C(0x5E)
#define	BMA4_INTERNAL_ERROR			UINT8_C(0x5F)

/**\name Feature offset address */
#define BMA423_ANY_NO_MOTION_OFFSET		UINT8_C(0x00)
#define BMA423_STEP_CNTR_OFFSET			UINT8_C(0x36)
#define BMA423_STEP_CNTR_PARAM_OFFSET		UINT8_C(0x04)
#define BMA423_WAKEUP_OFFSET			UINT8_C(0x38)
#define BMA423_TILT_OFFSET			UINT8_C(0x3A)
#define BMA423_CONFIG_ID_OFFSET			UINT8_C(0x3C)
#define BMA423_AXES_REMAP_OFFSET		UINT8_C(0x3E)

/**\name POWER_CTRL REGISTER*/
#define	BMA4_POWER_CONF_ADDR	UINT8_C(0x7C)
#define	BMA4_POWER_CTRL_ADDR	UINT8_C(0x7D)

#define BMA4_ACCEL_CONFIG_ADDR UINT8_C(0x40)

/**\name ADVANCE POWER SAVE POSITION AND MASK*/
#define	BMA4_ADVANCE_POWER_SAVE_MSK	UINT8_C(0x01)

/**\name AUX/ACCEL DATA BASE ADDRESS REGISTERS*/
#define  BMA4_DATA_0_ADDR  UINT8_C(0X0A)
#define BMA4_DATA_8_ADDR  UINT8_C(0X12)


// Commonly used values
#define BMA4_ENABLE_ACCEL_DATA_ACQUISITION 0x04
#define BMA4_DISABLE_PERF_MODE_SAMPLING50 0x17
enum Coefficients
{
  BMA400_STATUS		=		0x03,

  BMA400_ACC_X_LSB	=		0x12,
  BMA400_ACC_X_MSB	=		0x13,
  BMA400_ACC_Y_LSB	=		0x14,
  BMA400_ACC_Y_MSB	=		0x15,
  BMA400_ACC_Z_LSB	=		0x16,
  BMA400_ACC_Z_MSB	=		0x17,

  BMA400_TEMP_ADDR	=		0x11,
  BMA400_ACC_CONFIG0  =		0x19,
  BMA400_ACC_CONFIG1  =		0x1A,

};

#define BMA4_SET_BITS_POS_0(reg_data, bitname, data) ((reg_data & ~(bitname##_MSK)) | (data & bitname##_MSK))


struct DeviceParameter
{
  uint8_t I2CAddress;
  uint8_t powerMode;
  uint8_t measurementRange;
  uint8_t outputDataRate;
  uint8_t oversamplingRate;
  int16_t raw_acc_x;
  int16_t raw_acc_y;
  int16_t raw_acc_z;
  float acc_x;
  float acc_y;
  float acc_z;
};


class BMA421 {
public:
  DeviceParameter parameter;



  BMA421();
  uint8_t init(void);
  uint8_t checkID(void);
  int writeConfigFile(void);
  void writeConfigStream(uint8_t data, uint16_t index);
  void enableSensorFeatures();
  void setPowerMode(uint8_t mode);
  void enableStepCounter(bool enable);
  uint8_t getCurrentSteps();
  uint8_t isUserWalking();
  uint8_t resetCurrentSteps();
  uint8_t getAccelData();
  void setAdvancedPowerSave(uint8_t power_mode);





  void readData();
  void readData_OLD();
  void convertData();

  uint8_t readByte(byte reg);
  void writeByte(byte reg, byte value);


};
