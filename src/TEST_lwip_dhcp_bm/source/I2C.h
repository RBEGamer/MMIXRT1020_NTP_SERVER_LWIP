#ifndef __I2C_H__
#define __I2C_H__

#include "fsl_lpi2c.h"

#define EXAMPLE_I2C_MASTER_BASE (LPI2C1_BASE)
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY
#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define I2C_BAUDRATE 100000U

static lpi2c_master_config_t masterConfig;


void i2c_init_alt();
int i2c_scan_alt();
bool i2c_read_alt(uint8_t deviceAddr, uint8_t reg, uint8_t bytes, uint8_t *in);
bool i2c_read_alt2(uint8_t deviceAddr, uint8_t bytes, uint8_t *in);
bool i2c_write_alt2(uint8_t deviceAddr, uint8_t bytes, uint8_t *out);
// overloadded function with register
bool i2c_write_alt(uint8_t deviceAddr, uint8_t reg, uint8_t bytes, uint8_t *out);


#endif //_I2C_H__
