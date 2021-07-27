#include "PCF85063AT.h"

#define REG_CTRL1_ADDR                   0x00
#define REG_CTRL2_ADDR                   0x01
#define REG_TIME_DATE_ADDR               0x04
#define REG_ALARM_ADDR                   0x0B
#define REG_OFFSET_ADDR                  0x02
#define REG_COUNTDOWN_TIMER_VALUE_ADDR   0x10
#define REG_COUNTDOWN_TIMER_MODE_ADDR    0x11


uint8_t bcd_decode(uint8_t bcd)
{
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t bcd_encode(uint8_t dec)
{
  return ((dec / 10) << 4) | (dec % 10);
}

void PCF85063AT_init()
{
  //i2c_init_alt();
}

bool PCF85063AT_time_get(datetime_t *now)
{
  uint8_t buf[7];

  if (!i2c_read_alt(PCF85063AT_I2C_ADDR, REG_TIME_DATE_ADDR, sizeof(buf), buf))
    return false;

  now->second = bcd_decode(buf[0] & ~0x80);
  now->minute = bcd_decode(buf[1] & ~0x80);
  now->hour   = bcd_decode(buf[2] & ~0xC0); /* 24h clock */
  now->day    = bcd_decode(buf[3] & ~0xC0);
  now->wday   = bcd_decode(buf[4] & ~0xF8);
  now->month  = bcd_decode(buf[5] & ~0xE0);
  now->year   = bcd_decode(buf[6]);

  return !(buf[0] & 0x80);
}

bool PCF85063AT_time_set(datetime_t *new_time)
{
  uint8_t buf[7];

  buf[0] = bcd_encode(new_time->second);
  buf[1] = bcd_encode(new_time->minute);
  buf[2] = bcd_encode(new_time->hour);
  buf[3] = bcd_encode(new_time->day);
  buf[4] = bcd_encode(new_time->wday);
  buf[5] = bcd_encode(new_time->month);
  buf[6] = bcd_encode(new_time->year);

  return i2c_write_alt(PCF85063AT_I2C_ADDR, REG_TIME_DATE_ADDR, sizeof(buf), buf);
}

bool PCF85063AT_reset()
{
  uint8_t buf = 0x58;
  return i2c_write_alt(PCF85063AT_I2C_ADDR, REG_CTRL1_ADDR, 1, &buf);
}

bool PCF85063AT_stop(bool stopped)
{
  PCF85063AT_Regs regs;

  if (!PCF85063AT_ctrl_get(&regs))
    return false;

  if (stopped) PCF85063AT_REG_SET(regs, PCF85063AT_REG_STOP);
  else         PCF85063AT_REG_CLEAR(regs, PCF85063AT_REG_STOP);

  return PCF85063AT_ctrl_set(regs, true);
}

bool PCF85063AT_clkout_freq_set(uint16_t freq)
{
  uint8_t COF;
  PCF85063AT_Regs regs;

  switch (freq)
  {
    case 0:     COF = 7; break;
    case 1:     COF = 6; break;
    case 1024:  COF = 5; break;
    case 2048:  COF = 4; break;
    case 4096:  COF = 3; break;
    case 8192:  COF = 2; break;
    case 16384: COF = 1; break;
    case 32768: COF = 0; break;
    default: return false;
  }

  if (!PCF85063AT_ctrl_get(&regs))
    return false;

  PCF85063AT_REG_CLEAR(regs, PCF85063AT_REG_COF);
  regs |= ((uint16_t)COF) << 8;

  return PCF85063AT_ctrl_set(regs, true);
}

bool PCF85063AT_countdown_set(bool enable,
                         CountdownSrcClock source_clock,
                         uint8_t value,
                         bool int_enable,
                         bool int_pulse)
{
  uint8_t timer_reg[2] = {0};

  if (source_clock < 0 || source_clock > 3)
    return false;

  /* First disable the countdown timer */
  if (!i2c_write_alt(PCF85063AT_I2C_ADDR, REG_COUNTDOWN_TIMER_MODE_ADDR, 1, timer_reg+1))
    return false;

  /* Reconfigure timer */
  if (enable) timer_reg[1] |= PCF85063AT_REG_TE;
  if (int_enable) timer_reg[1] |= PCF85063AT_REG_TIE;
  if (int_pulse) timer_reg[1] |= PCF85063AT_REG_TI_TP;
  timer_reg[1] |= source_clock << 3;
  timer_reg[0] = value;

  return i2c_write_alt(PCF85063AT_I2C_ADDR, REG_COUNTDOWN_TIMER_VALUE_ADDR, 2, timer_reg);
}

bool PCF85063AT_countdown_get(uint8_t *value)
{
  return i2c_read_alt(PCF85063AT_I2C_ADDR, REG_COUNTDOWN_TIMER_VALUE_ADDR, 1, value);
}

bool PCF85063AT_alarm_set(int second, int minute, int hour, int day,
                     int weekday)
{
  uint8_t buf[5];

  if ((second < 0 || second > 59) && second != -1) return false;
  if ((minute < 0 || minute > 59) && minute != -1) return false;
  if ((hour < 0 || hour > 23) && hour != -1) return false;
  if ((day < 0 || day > 31) && day != -1) return false;
  if ((weekday < 0 || weekday > 6) && weekday != -1) return false;

  buf[0] = second < 0 ? 0x80 : bcd_encode(second);
  buf[1] = minute < 0 ? 0x80 : bcd_encode(minute);
  buf[2] = hour < 0 ? 0x80 : bcd_encode(hour);
  buf[3] = day < 0 ? 0x80 : bcd_encode(day);
  buf[4] = weekday < 0 ? 0x80 : bcd_encode(weekday);

  return i2c_write_alt(PCF85063AT_I2C_ADDR, REG_ALARM_ADDR, sizeof(buf), buf);
}


bool PCF85063AT_ctrl_get(PCF85063AT_Regs *regs)
{
  uint8_t buf[2];

  if (!i2c_read_alt(PCF85063AT_I2C_ADDR, REG_CTRL1_ADDR, sizeof(buf), buf))
    return false;

  *regs = buf[0];
  *regs |= ((uint16_t)buf[1]) << 8;

  return true;
}

bool PCF85063AT_ctrl_set(PCF85063AT_Regs regs, bool mask_alarms)
{
  uint8_t buf[2];
  //int wrsz;

  if (mask_alarms)
    regs &= ~(PCF85063AT_REG_AF | PCF85063AT_REG_TF);

  buf[0] = regs & 0xFF;
  buf[1] = regs >> 8;

  return i2c_write_alt(PCF85063AT_I2C_ADDR, REG_CTRL1_ADDR, sizeof(buf), buf);
}

