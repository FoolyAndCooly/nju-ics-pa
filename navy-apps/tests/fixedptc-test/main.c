#include"/home/liushao/ics2022/navy-apps/libs/libfixedptc/include/fixedptc.h"
#include <stdio.h>
int main(){
fixedpt a = fixedpt_rconst(1.2);
fixedpt b = fixedpt_fromint(10);
int c = 0;
  c = fixedpt_toint(fixedpt_ceil(a));

printf("%d\n",c);
}
