#include <stdio.h>
#include <sys/time.h>
int main(){
  //AM_TIMER_RTC_T rtc;
  int sec = 1;
  struct timeval tv;
  while (1) {
    gettimeofday(&tv,NULL);
    //while( tv.tv_sec < sec) ;
    //rtc = io_read(AM_TIMER_RTC);
    //printf("%d-%d-%d %02d:%02d:%02d GMT (", rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
      printf("(%d seconds).\n", tv.tv_sec);
    
    sec ++;
  }
}
