#include <stdio.h>
#include <sys/time.h>
#include "/home/liushao/ics2022/navy-apps/libs/libndl/include/NDL.h"
int main(){
  //AM_TIMER_RTC_T rtc;
  int sec = 1;
  //struct timeval tv;
  while (1) {
    while( NDL_GetTicks() < sec){;}
    //rtc = io_read(AM_TIMER_RTC);
    //printf("%d-%d-%d %02d:%02d:%02d GMT (", rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
      printf("(%d seconds).\n",sec);
    
    sec ++;
  }
}
