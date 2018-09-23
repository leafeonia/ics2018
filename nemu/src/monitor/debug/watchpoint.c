#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
	if(free_ == NULL){
		panic("wp_pool is out of space.");
	}
	else{
		WP* x = free_;
		x->next = NULL;
		free_ = x->next;
		if(head == NULL){
			head = x;
		}
		else{
			head->next = x;
		}
		return x;
	}
}

void free_wp(WP *wp){
	printf("free wp%d\n",wp->NO);
	wp->next = free_;
	free_ = wp;
}

void del(int num){
	WP* cur = head;
	if(cur->NO == num){
		printf("head->NO = num\n");
		head = head->next;
		free_wp(cur);
	}
	else{
		bool found = false;
		while(cur->next){
			if(cur->next->NO == num){
				found = true; 
				WP* temp = cur->next;
				cur->next = cur->next->next;
				printf("wp%d is free now. Previous: wp%d, next: wp%d.\n",cur->NO,temp->NO,cur->next->NO);
				free_wp(temp);
			}
		}
		if(!found) Log("NO watchpoint %d\n",num);
	}
}

void wp_info(){
	printf("NO\texpression\tcurrent value\tnext watchpoint\n");
	WP* cur = head;
	while(cur){
		printf("%d\t%s\t%d\n",cur->NO,cur->exp,cur->value);
		cur = cur->next;
	}
}

