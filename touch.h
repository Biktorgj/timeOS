/* Touch panel */
#define TP_RESET 10
#define TP_INT 28 // input
#define HRM_TEST 30 // input

#define FLAGBIT(x)              (0x00000001 << (x))
#define FLAGBITS(x, y)          ((0xFFFFFFFF >> (32 - (y) - 1)) << (x))

#define I2C_BUFFER_LENGTH_MAXINUM           256
#define FILE_NAME_LENGTH                    128
#define ENABLE                              1
#define DISABLE                             0
/*register address*/
#define HYN_REG_INT_CNT                     0x8F
#define HYN_REG_FLOW_WORK_CNT               0x91
#define HYN_REG_WORKMODE                    0x00
#define HYN_REG_WORKMODE_FACTORY_VALUE      0x40
#define HYN_REG_WORKMODE_WORK_VALUE         0x00
#define HYN_REG_CHIP_ID                     0xA3
#define HYN_REG_CHIP_ID2                    0x9F
#define HYN_REG_POWER_MODE                  0xA5
#define HYN_REG_POWER_MODE_SLEEP_VALUE      0x03
#define HYN_REG_FW_VER                      0xA6
#define HYN_REG_VENDOR_ID                   0xA8
#define HYN_REG_LCD_BUSY_NUM                0xAB
#define HYN_REG_FACE_DEC_MODE_EN            0xB0
#define HYN_REG_GLOVE_MODE_EN               0xC0
#define HYN_REG_COVER_MODE_EN               0xC1
#define HYN_REG_CHARGER_MODE_EN             0x8B
#define HYN_REG_GESTURE_EN                  0xD0
#define HYN_REG_GESTURE_OUTPUT_ADDRESS      0xD3
#define HYN_REG_ESD_SATURATE                0xED

class Touch
{
  public:
    Touch();

  private:
    void setupInterrupt();
};
