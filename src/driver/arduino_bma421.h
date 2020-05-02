/*
    A library for Grove - Step Counter(BMA421)

    Copyright (c 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : June 2018
    Change Log  :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software", to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __BOSCH_BMA421_H__
#define __BOSCH_BMA421_H__

#include <Arduino.h>
#include <Wire.h>
#include "bma421.h"

typedef enum {
    CIC_AVG = BMA4_CIC_AVG_MODE,
    CONTINUOUS = BMA4_CONTINUOUS_MODE,
} MA421_PERF_MODE;

typedef enum {
    OSR4_AVG1 = BMA4_ACCEL_OSR4_AVG1,
    OSR2_AVG2 = BMA4_ACCEL_OSR2_AVG2,
    NORMAL_AVG4 = BMA4_ACCEL_NORMAL_AVG4,
    CIC_AVG8 = BMA4_ACCEL_CIC_AVG8,
    RES_AVG16 = BMA4_ACCEL_RES_AVG16,
    RES_AVG32 = BMA4_ACCEL_RES_AVG32,
    RES_AVG64 = BMA4_ACCEL_RES_AVG64,
    RES_AVG128 = BMA4_ACCEL_RES_AVG128,
} MA421_BW;

typedef enum {
    ODR_0_78_HZ = BMA4_OUTPUT_DATA_RATE_0_78HZ,
    ODR_1_5_HZ = BMA4_OUTPUT_DATA_RATE_1_56HZ,
    ODR_3_1_HZ = BMA4_OUTPUT_DATA_RATE_3_12HZ,
    ODR_6_25_HZ = BMA4_OUTPUT_DATA_RATE_6_25HZ,
    ODR_12_5_HZ = BMA4_OUTPUT_DATA_RATE_12_5HZ,
    ODR_25_HZ = BMA4_OUTPUT_DATA_RATE_25HZ,
    ODR_50_HZ = BMA4_OUTPUT_DATA_RATE_50HZ,
    ODR_100_HZ = BMA4_OUTPUT_DATA_RATE_100HZ,
    ODR_200_HZ = BMA4_OUTPUT_DATA_RATE_200HZ,
    ODR_400_HZ = BMA4_OUTPUT_DATA_RATE_400HZ,
    ODR_800_HZ = BMA4_OUTPUT_DATA_RATE_800HZ,
    ODR_1600_HZ = BMA4_OUTPUT_DATA_RATE_1600HZ,
} MBA421_ODR;

typedef enum {
    RANGE_2G = BMA4_ACCEL_RANGE_2G,
    RANGE_4G = BMA4_ACCEL_RANGE_4G,
    RANGE_8G = BMA4_ACCEL_RANGE_8G,
    RANGE_16G = BMA4_ACCEL_RANGE_16G,
} MA421_RANGE;

typedef enum {
    PHONE_CONFIG = 0,
    WRIST_CONFIG = 1,
} MA421_PLATFORM_CONF;

class BMA421 {
  public:

    BMA421(void) {}

    uint16_t initialize(MA421_RANGE range = RANGE_4G, MBA421_ODR odr = ODR_100_HZ,
                    MA421_BW bw = NORMAL_AVG4, MA421_PERF_MODE mode = CIC_AVG);

    void stepCounterEnable(MA421_PLATFORM_CONF conf = WRIST_CONFIG, int cmd = true);

    void getAcceleration(float* x, float* y, float* z);

    int32_t getTemperature(void);

    uint32_t getStepCounterOutput(void);

  private:

    float devRange;
    struct bma4_dev accel;
    struct bma4_accel_config accel_conf;

};

extern BMA421 bma421;

#endif
