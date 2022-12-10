#include"/home/liushao/ics2022/navy-apps/libs/libfixedptc/include/fixedptc.h"
#include <stdio.h>
int main(){
fixedpt a = fixedpt_rconst(-1.2);
fixedpt b = fixedpt_fromint(10);
int c = 0;
if (b > fixedpt_rconst(7.9)) {
  c = fixedpt_toint(fixedpt_div(fixedpt_mul(fixedpt_abs(a) + FIXEDPT_ONE, b), fixedpt_rconst(2.3)));
}

printf("%d\n",c);
}
