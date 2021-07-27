#ifndef __RTC_PCF85063AT_H__
#define __RTC_PCF85063AT_H__

#include "datetime.h"
#include "I2C.h"

#define PCF85063AT_I2C_ADDR   0x51
/* Type for holding PCF85063A control registers 1 and 2 */
typedef uint16_t PCF85063AT_Regs;

/* Control register 1 */
#define PCF85063AT_REG_EXT_TEST          (uint16_t)0x0080
#define PCF85063AT_REG_STOP              (uint16_t)0x0020
#define PCF85063AT_REG_SR                (uint16_t)0x0010
#define PCF85063AT_REG_CIE               (uint16_t)0x0004
#define PCF85063AT_REG_12_24             (uint16_t)0x0002
#define PCF85063AT_REG_CAP_SEL           (uint16_t)0x0001

/* Control register 2 */
#define PCF85063AT_REG_AIE               (uint16_t)0x8000
#define PCF85063AT_REG_AF                (uint16_t)0x4000
#define PCF85063AT_REG_MI                (uint16_t)0x2000
#define PCF85063AT_REG_HMI               (uint16_t)0x1000
#define PCF85063AT_REG_TF                (uint16_t)0x0800
#define PCF85063AT_REG_COF               (uint16_t)0x0700

/* Timer mode register - not part of PCF85063A_Regs */
#define PCF85063AT_REG_TCF               (uint16_t)0x08
#define PCF85063AT_REG_TE                (uint16_t)0x04
#define PCF85063AT_REG_TIE               (uint16_t)0x02
#define PCF85063AT_REG_TI_TP             (uint16_t)0x01

#define PCF85063AT_REG_GET(regs, reg) (!!((regs) & (reg)))
#define PCF85063AT_REG_SET(regs, reg) do { (regs) |= (reg); } while(0)
#define PCF85063AT_REG_CLEAR(regs, reg) do { (regs) &= ~(reg); } while(0)


//uint8_t bcd_decode(uint8_t bcd);

//uint8_t bcd_encode(uint8_t dec);

typedef enum {
	CNTDOWN_CLOCK_4096HZ   = 0,
	CNTDOWN_CLOCK_64HZ     = 1,
	CNTDOWN_CLOCK_1HZ      = 2,
	CNTDOWN_CLOCK_1PER60HZ = 3
} CountdownSrcClock;
void PCF85063AT_init();
bool PCF85063AT_time_get(datetime_t *now);
bool PCF85063AT_time_set(datetime_t *new_time);
bool PCF85063AT_reset();
bool PCF85063AT_stop(bool stopped);
bool PCF85063AT_clkout_freq_set(uint16_t freq);
bool PCF85063AT_countdown_set(bool enable, CountdownSrcClock source_clock, uint8_t value, bool int_enable, bool int_pulse);
bool PCF85063AT_countdown_get(uint8_t *value);
bool PCF85063AT_alarm_set(int second, int minute, int hour, int day, int weekday);
bool PCF85063AT_ctrl_get(PCF85063AT_Regs *regs);
bool PCF85063AT_ctrl_set(PCF85063AT_Regs regs, bool mask_alarms);

#endif //__RTC_PCF85063AT_H__
