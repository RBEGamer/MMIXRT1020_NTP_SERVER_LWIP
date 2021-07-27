#ifndef __DATETIME_H__
#define __DATETIME_H__
#include <stdint.h>

#define LEAP_YEAR(Y)  ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )
#define SECS_PER_MIN  ((timestamp_t)(60UL))
#define SECS_PER_HOUR ((timestamp_t)(3600UL))
#define SECS_PER_DAY  ((timestamp_t)(SECS_PER_HOUR * 24UL))
#define DAYS_PER_WEEK ((timestamp_t)(7UL))
#define SECS_PER_WEEK ((timestamp_t)(SECS_PER_DAY * DAYS_PER_WEEK))
#define SECS_PER_YEAR ((timestamp_t)(SECS_PER_DAY * 365UL)) // TODO: ought to handle leap years
#define SECS_YR_2000  ((timestamp_t)(946684800UL)) // the time at the start of y2k


typedef unsigned long timestamp_t;

typedef struct  { 
  uint8_t second; 
  uint8_t minute; 
  uint8_t hour; 
  uint8_t wday;   // day of week, sunday is day 1
  uint8_t day;
  uint8_t month; 
  uint8_t year;   // offset from 1970; 
} datetime_t; 


static const uint8_t monthDays[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // API starts months from 1, this array starts from 0

void breakTime(timestamp_t ts, datetime_t *dt);
timestamp_t makeTime(const datetime_t dt);

#endif //__DATETIME_H__

