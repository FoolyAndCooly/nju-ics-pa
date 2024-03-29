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

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
enum {
  TK_NOTYPE = 1, TK_EQ, TK_NUMBER, TK_NEQ, TK_AND, TK_P, TK_REG, TK_HNUM,TK_N

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  {"\\(", '('},
  {"\\)",')'},
  {"==", TK_EQ},  // equal
  {"!=",TK_NEQ},
  {"&&",TK_AND},
  {"[$a-zA-Z]+[0-9]*",TK_REG},
  {"0[xX][0-9a-fA-F]+",TK_HNUM},
  {"[0-9]+",TK_NUMBER},
  {"\\*",TK_P}
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[100] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i,j,k;
  regmatch_t pmatch;

  nr_token = 0;
  
  while (e[position] != '\0') {
  	//printf("%c",*(e+position));
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            //i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
	case TK_NOTYPE:break;
	case TK_HNUM:substr_start+=2;
	case TK_REG:
	case TK_NUMBER:
	for(j=0;j<substr_len;j++){
		tokens[nr_token].str[j]=*(substr_start+j);}
		tokens[nr_token].str[j]='\0';
          default: tokens[nr_token].type=rules[i].token_type;nr_token++;
		   //Log("write into tokens[%d] successfully,%d",nr_token-1,tokens[nr_token-1].type);
        }
	for (k = 0; k < nr_token; k ++) {
 	 if (tokens[k].type == '*' && (k==0||tokens[k - 1].type == '+'||tokens[k - 1].type =='-'||tokens[k - 1].type =='*'||tokens[k - 1].type =='/'|| tokens[k-1].type == TK_EQ||tokens[k-1].type ==TK_NEQ||tokens[k-1].type ==TK_AND || tokens[k-1].type=='(')) {
    		tokens[k].type = TK_P;
		//Log("change tokens[%d] from *  to TK_P",i);
  		}
         if (tokens[k].type == '-' && (k==0 || tokens[k - 1].type == '+'||tokens[k - 1].type =='-'||tokens[k - 1].type =='*'||tokens[k - 1].type =='/'|| tokens[k-1].type == TK_EQ||tokens[k-1].type ==TK_NEQ||tokens[k-1].type ==TK_AND || tokens[k-1].type=='(')) {
                tokens[k].type = TK_P;
                //Log("change tokens[%d] from -  to TK_N",k);
                }

	}
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p, int q){
	int i,cnt=0;
	if (tokens[p].type!='(') return false;
	for(i=p;i<=q;i++){
	if (tokens[i].type=='(') cnt++;
	else if(tokens[i].type==')') cnt--;
	if(cnt==0 && i!=q) return false;
	}
       return true;	
}
bool exclu(int *a,int *b,int po,int cnt){
	//Log("%d",*a);
	//Log("%d",*b);
	int i;
	for(i=0;i<=cnt;i++){
	if(po>=a[i] && po<=b[i]){ return false;}
	}
	return true;
}
uint32_t search_op(int p, int q){
	int i;
	int a[20],b[20];
	char wait[100]={0};
	int waitn[100]={0};
	int cnta=-1,cnt1=0;
	bool ch=false;
	for (i=p;i<=q;i++){
		if(tokens[i].type=='('){
		if(!ch){cnta++;a[cnta]=i;ch=true;}
		cnt1++;}
		if(tokens[i].type==')'){
		cnt1--;
		if(cnt1 == 0){b[cnta]=i;ch=false;}
		}
		}		
	for (i=p;i<=q;i++){
		if(tokens[i].type != TK_NUMBER && exclu(a,b,i,cnta)){
		wait[cnt1]=tokens[i].type;
		waitn[cnt1]=i;
		cnt1++;
		//Log("write %d into wait[%d] succussfully",tokens[i].type,cnt1);
		}
	}
        for (i=cnt1-1;i>=0;i--){
	if (wait[i] == TK_AND || wait[i]== TK_EQ ||wait[i]== TK_NEQ){return waitn[i];}
	}
	for (i=cnt1-1;i>=0;i--){
        if (wait[i] == '+' || wait[i]=='-') {return waitn[i];}//Log("op1");;
        }
        for (i=cnt1-1;i>=0;i--){
        if (wait[i] == '*' || wait[i]== '/' ){return waitn[i];}// Log("op2");;
        }
	//Log("can't find op,%d",wait[0]);
	return 0;
}

uint32_t eval(int p,int q){
	int a;
	if (p >q){
		printf("Bad expression");
		assert(0);
	}
        else if (p+1 ==q && (tokens[p].type == TK_P && tokens[q].type == TK_REG)){
                bool success=true;
                int val0;
                val0=isa_reg_str2val(tokens[q].str, &success);
                if (success) return val0;
                else Log("reg not found");
                }
       else if (p+1 ==q || (tokens[p].type == TK_N && (tokens[q].type == TK_NUMBER || tokens[q].type == TK_HNUM))){
                switch (tokens[q].type){
                case TK_NUMBER: return 0-atoi(tokens[q].str);
                case TK_HNUM:
                sscanf(tokens[q].str,"%x",&a);
                return 0-a;
                default:assert(0);
                }
                }
	else if (p == q){
		//if(tokens[p].type != TK_NUMBER && tokens[p].type != TK_HNUM){
		//	printf("Bad expression");
		//	assert(0);
		//}
		//return atoi(token[p].str)
		
		switch (tokens[p].type){
		case TK_NUMBER: return atoi(tokens[p].str);
		case TK_HNUM: 
		sscanf(tokens[p].str,"%x",&a);
		return a;
		//default:assert(0);
		}
	}
	else if (check_parentheses(p,q) == true){
		return eval(p+1,q-1);
	}


	else{
		uint32_t op;
		int val1,val2;
		op = search_op(p,q);
		val1 = eval(p,op-1);
		val2 = eval(op+1,q);

		switch (tokens[op].type){
		case '+': return val1 + val2;
      		case '-': return val1 - val2;
      		case '*': return val1 * val2;
      		case '/': return val1 / val2;
		case TK_EQ: return val1 == val2;
		case TK_NEQ: return val1 != val2;
		case TK_AND: return val1 && val2;
      		default: assert(0);
		}
	}
	return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  return eval(0,nr_token-1);
}
