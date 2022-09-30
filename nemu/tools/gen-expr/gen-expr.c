/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
char* pbuf=buf;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static uint32_t choose(uint32_t n){
	return rand() % n;
}
static void gen_num(){
	pbuf+=sprintf(pbuf,"%d",rand()%50);
	
}
static void gen(char f){
	pbuf+=sprintf(pbuf,"%c",f);
}
static void gen_rand_op(){
	char f[4]={'+','-','*','/'};
	pbuf+=sprintf(pbuf,"%c",f[choose(4)]);
}
static void gen_blank(){
	for (int j=1;j<rand()%3;j++) pbuf+=sprintf(pbuf,"%c",' ');
}
static void gen_rand_expr() {
	//buf[0]='\0';
  switch (choose(3)){
	  case 0: gen_blank();gen_num();gen_blank(); break;
	  case 1: gen_blank();gen('('); gen_rand_expr(); gen(')');gen_blank(); break;
	  default :gen_rand_expr(); gen_rand_op();gen_rand_expr(); break;
  }
}

static void gen_my_expr(){
	char my_expr[100];
	scanf("%s",my_expr);
	pbuf+=sprintf(pbuf,"%s",my_expr);
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  int  way;
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  scanf("%d",&way);
  for (i = 0; i < loop; i ++) {
    if(way){
    gen_rand_expr();
    }
    else{
    gen_my_expr();
    }
    *pbuf='\0';
    pbuf=buf;
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
