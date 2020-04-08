#include "bma421.h"
#include "bma421_config.h"
#include <Wire.h>


BMA421::BMA421()
{
	parameter.I2CAddress;
	parameter.powerMode;
	parameter.measurementRange;
}


/* Initialize the module */
uint8_t BMA421::init(void)
{
	Wire.begin();
	return checkID();
}

//##########################################################################
// Reworked functions
//##########################################################################
uint8_t BMA421::checkID(void)
{
	uint8_t chipID;
	chipID = readByte(BMA400_CHIP_ID);
	return chipID;

}
/* Write the config file:
1. Disable power saving
2. Set init control address to 0
3. Dump the entire configuration profile to the chip
4. Write 0x01 to the init control address
5. Enable advanced power save and exit
*/
int BMA421::writeConfigFile(void) {
	uint16_t i;
	uint16_t cfg_stream_status = 0;
	setAdvancedPowerSave(BMA4_DISABLE);
	writeByte(BMA4_INIT_CTRL_ADDR, 0x00); // Disable config loading

	for (i = 0; i < BMA4_CONFIG_STREAM_SIZE; i++) {
		writeConfigStream(bma423_config_file[i], i); // dump the config file
	}

	writeByte(BMA4_INIT_CTRL_ADDR, 0x01); // Enable config load and fifo mode
	delay(150); // Give it a little time to digest all the data we put in
	cfg_stream_status = readByte(BMA4_INTERNAL_STAT);
	if (cfg_stream_status != BMA4_ASIC_INITIALIZED) {
		return 0;
	} else {
		setAdvancedPowerSave(BMA4_ENABLE);
		return 1;
	}
}

void BMA421::writeConfigStream(uint8_t data, uint16_t index) {
	uint8_t asic_msb = (uint8_t)((index / 2) >> 4);
	uint8_t asic_lsb = ((index / 2) & 0x0F);
	writeByte(BMA4_RESERVED_REG_5B_ADDR, asic_lsb);
	writeByte(BMA4_RESERVED_REG_5C_ADDR, asic_msb);
	writeByte(BMA4_FEATURE_CONFIG_ADDR, data);
}

void BMA421::enableSensorFeatures() {
	writeByte(BMA4_INIT_CTRL_ADDR, 0x01); // Enable sensor features
}
void BMA421::setPowerMode(uint8_t mode) {

}
/* STEP COUNTING */
// Enable or disable step counter mode
void BMA421::enableStepCounter(bool enable) {

}
// Get current steps since last reset
uint8_t BMA421::getCurrentSteps() {

}
// Is user Walking, running, or stationary
uint8_t BMA421::isUserWalking() {

}
// Reset bma's step registry
uint8_t BMA421::resetCurrentSteps() {
	uint8_t feature_config[BMA423_FEATURE_SIZE] = {0};
	/* Reset bit is 1 byte ahead of base address */
	uint8_t index = BMA423_STEP_CNTR_OFFSET + 1;
	uint16_t data = 0x00;

}
// Get raw accel data
uint8_t BMA421::getAccelData() {

}

/*
*
*
*
*/
void BMA421::setAdvancedPowerSave(uint8_t power_mode) {
	// Check current power state and change it
	uint16_t rslt = 0;
	uint8_t data = 0;

	data = readByte(BMA4_POWER_CONF_ADDR);
	data = BMA4_SET_BITS_POS_0(data, BMA4_ADVANCE_POWER_SAVE, power_mode);
	writeByte(BMA4_POWER_CONF_ADDR, power_mode);

}

void BMA421::readData(void) {
	uint8_t rawData[6];
	Wire.beginTransmission(parameter.I2CAddress);
	Wire.write(BMA4_POWER_CTRL_ADDR);
	Wire.write(BMA4_ENABLE_ACCEL_DATA_ACQUISITION);
	Wire.write(BMA4_ACCEL_CONFIG_ADDR);
	Wire.write(BMA4_DISABLE_PERF_MODE_SAMPLING50);

	Wire.write(BMA4_POWER_CONF_ADDR);
	Wire.write(0x03);


	Wire.write(BMA4_DATA_8_ADDR);
	Wire.endTransmission();
	Wire.requestFrom(parameter.I2CAddress, 6);

	uint8_t acc_x_lsb = Wire.read();
	uint8_t acc_x_msb = Wire.read();
	uint8_t acc_y_lsb = Wire.read();
	uint8_t acc_y_msb = Wire.read();
	uint8_t acc_z_lsb = Wire.read();
	uint8_t acc_z_msb = Wire.read();


	parameter.raw_acc_x = acc_x_lsb + (256)*acc_x_msb;
	if (parameter.raw_acc_x > 2047)
	parameter.raw_acc_x = parameter.raw_acc_x - 4096;

	parameter.raw_acc_y = acc_y_lsb + (256)*acc_y_msb;
	if (parameter.raw_acc_y > 2047)
	parameter.raw_acc_y = parameter.raw_acc_y - 4096;

	parameter.raw_acc_z = acc_z_lsb | (256)*acc_z_msb;
	if (parameter.raw_acc_z > 2047)
	parameter.raw_acc_z = parameter.raw_acc_z - 4096;

	//	convertData();

}


//##########################################################################
//DATA READ FUNCTIONS
//##########################################################################
void BMA421::convertData(void)
{
	float range;
	if (parameter.measurementRange == 0)
	range = 2000;

	if (parameter.measurementRange == 1)
	range = 4000;

	if (parameter.measurementRange == 2)
	range = 8000;

	if (parameter.measurementRange == 3)
	range = 16000;

	parameter.acc_x = parameter.raw_acc_x*range/2048;
	parameter.acc_y = parameter.raw_acc_y*range/2048;
	parameter.acc_z = parameter.raw_acc_z*range/2048;


}
//##########################################################################
//BASIC FUNCTIONS
//##########################################################################
void BMA421::writeByte(byte reg, byte value)
{
	Wire.beginTransmission(parameter.I2CAddress);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}
//##########################################################################
uint8_t BMA421::readByte(byte reg)
{
	uint8_t value;
	Wire.beginTransmission(parameter.I2CAddress);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(parameter.I2CAddress,1);
	value = Wire.read();
	return value;
}
