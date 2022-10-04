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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char str[32];
  int val;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}
static bool in=false;

/* TODO: Implement the functionality of watchpoint */
void new_wp(char* expr,int val0){
	WP* new,*be;
	if(!in){
	init_wp_pool();
	in=true;
	}
	be=NULL;
	new=free_;
	new->val=val0;
	Log("0");
	strcpy(new->str,expr);
	Log("1");
	new->next=NULL;
	free_=free_->next;
	if (head ==NULL){head=new;}
	else{
		be=head;
		while(be->next!=NULL){be=be->next;}
		be->next=new;
	}
	printf("add the watchpoint %d :%s successfully\n",new->NO,new->str);

}
void free_wp(int nu){
	WP *be1,*wp;
	be1=head;
	wp=head;
	for(int i=0;i<nu;i++){
	wp=wp->next;
	}
	if(wp!=head){
	while(be1->next!=wp){be1=be1->next;}
	be1->next=NULL;
	}
	else{
	head=NULL;
	}
	wp->next=free_;
	free_=wp;
	printf("clear the watchpoint %d :%s successfully\n",wp->NO,wp->str);
}

void check_watch_point(){
  WP* wp0;
  wp0=head;
  bool su=true;
  while(wp0->next !=NULL && head!=NULL){
  printf("ok\n");
          if(wp0->val != expr(wp0->str,&su)){
                 printf("old val %d\n", expr(wp0->str,&su));
                printf("new val %d\n",wp0->val);
                nemu_state.state=NEMU_STOP;
          }
          wp0=wp0->next;
  }
}

