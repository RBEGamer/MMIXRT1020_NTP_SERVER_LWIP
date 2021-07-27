#include "datetime.h"

void breakTime(timestamp_t ts, datetime_t *dt) {
  uint8_t year;
  uint8_t month, monthLength;
  uint32_t time;
  unsigned long days;

  time = (uint32_t)ts;
  dt->second = time % 60;
  time /= 60; // now it is minutes
  dt->minute = time % 60;
  time /= 60; // now it is hours
  dt->hour = time % 24;
  time /= 24; // now it is days
  dt->wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  dt->year = year; // year is offset from 1970 
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    if (month==1) { // february
      if (LEAP_YEAR(year)) {
        monthLength=29;
      } else {
        monthLength=28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (time >= monthLength) {
      time -= monthLength;
    } else {
        break;
    }
  }
  dt->month = month + 1;  // jan is month 1  
  dt->day = time + 1;     // day of month
}


timestamp_t makeTime(const datetime_t dt) {   
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= dt.year*(SECS_PER_DAY * 365);
  for (i = 0; i < dt.year; i++) {
    if (LEAP_YEAR(i)) {
      seconds += SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < dt.month; i++) {
    if ( (i == 2) && LEAP_YEAR(dt.year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (dt.day-1) * SECS_PER_DAY;
  seconds+= dt.hour * SECS_PER_HOUR;
  seconds+= dt.minute * SECS_PER_MIN;
  seconds+= dt.second;

  return (timestamp_t)seconds; 
}

