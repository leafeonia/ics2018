#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#define min(a,b) (a < b ? a : b)

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_NUM = 100,
  PLUS = 1,
  MINUS = 2,
  MULTIPLY = 3,
  DIVIDE = 4,
  LEFT_BRACKET = 5,
  RIGHT_BRACKET = 6
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"[0-9]+",TK_NUM},
  {" +", TK_NOTYPE},    // spaces
  {"\\+", PLUS},         // plus
  {"-",MINUS},
  {"\\*",MULTIPLY},
  {"/",DIVIDE},
  {"\\(",LEFT_BRACKET},
  {"\\)",RIGHT_BRACKET},
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )
#define tokens_size 2560

static regex_t re[NR_REGEX];

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

Token tokens[tokens_size];
int nr_token;

static bool make_token(char *e) {
//	printf("Start make_token(). e = %s\n",e);
  memset(tokens,0,sizeof(tokens));
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;
  int cnt = 0;
  while (e[position] != '\0') {
    
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

      //  Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
      //      i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
		
		//printf("cnt = %d     rules[%d].token_type = %d\n",cnt,i,rules[i].token_type);
        switch (rules[i].token_type) {
          case 100:
    		tokens[cnt].type = 100;
    		if(substr_len > tokens_size){
    			Log("Warning : the number is too long to restore. Only 32 nums in the front are saved.");
    			}
    		for(int j = 0;j < min(substr_len,tokens_size);++j){
    			tokens[cnt].str[j] = substr_start[j];
    		}
    		cnt++;
    		for(int j = 0;j < 4;++j){
    		  // printf("tokens[%d].str[%d] = %d  ",cnt,j,tokens[cnt].str[j]);
  				if(!(j % 4)) printf("\n");
    		}
    		break; 
    	  case 1:
    		tokens[cnt++].type = 1;
    		break; 
    	  case 2:
    		tokens[cnt++].type = 2;
    		break; 
    	  case 3:
    		tokens[cnt++].type = 3;
    		break; 
    	  case 4:
    		tokens[cnt++].type = 4;
    		break; 
    	  case 5:
    		tokens[cnt++].type = 5;
    		break; 	
    	  case 6:
    		tokens[cnt++].type = 6;
    		break; 	
          default: 
          	break;
          	//TODO();
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

bool checkparentheses(int p,int q){
	if(tokens[p].type != LEFT_BRACKET || tokens[q].type != RIGHT_BRACKET) return false;
	int cnt = 0;
	for(int i = p + 1;i < q;i++){
		if(tokens[i].type == LEFT_BRACKET) cnt++;
		else if(tokens[i].type == RIGHT_BRACKET) cnt--;
		if(cnt < 0) return false;
	}
	if(cnt > 0)return false;
	return true;
}

uint32_t main_op(int p,int q){
	int priority[10];
	priority[PLUS] = priority[MINUS] = 1;
	priority[MULTIPLY] = priority[DIVIDE] = 2;
	int cur_priority = 3;
	int ans = 0;
	int bracket_cnt = 0;
	for(int i = p;i <= q;i++){
		if(tokens[i].type == LEFT_BRACKET) bracket_cnt++;
		else if(tokens[i].type == RIGHT_BRACKET) bracket_cnt--;
		if(bracket_cnt) continue;
		else if(bracket_cnt < 0){
			panic("Invalid input (bracket unmatched)");
		}
		if(PLUS <= tokens[i].type && tokens[i].type <= DIVIDE){
			if(priority[tokens[i].type] <= cur_priority){
				cur_priority = priority[tokens[i].type];
				ans = i;
			}
		}
	}
//	printf("main_op(%d,%d) = %d\n",p,q,ans);
	return ans;
}

uint32_t eval(int p,int q){
	//printf("eval(%d,%d)\n",p,q);
	if(p > q){
		panic("Something goes wrong here... in eval (%d,%d)",p,q);
	}
	else if(p == q){
		assert(tokens[p].type == TK_NUM);
	//	printf("str = %s, return : %d\n",tokens[p].str,atoi(tokens[p].str));
		return atoi(tokens[p].str);
	}
	else if(checkparentheses(p,q)){
		return eval(p+1,q-1); 
	}
	else{
		uint32_t op = main_op(p,q);
		uint32_t val1 = eval(p,op-1);
		uint32_t val2 = eval(op+1,q);
		switch(tokens[op].type){
			case PLUS:return val1 + val2;
			case MINUS:return val1 - val2;
			case MULTIPLY:return val1 * val2;
			case DIVIDE:{
				if(!val2) panic("error: div0");
				return val1 / val2;
			}
			default: assert(0);
		}
	}
}


uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  int length = 0;
  while(tokens[length].type) length++;
  
 /* for(int i = 0;i < 4;++i){
  	printf("tokens[%d].type = %d  ",i,tokens[i].type);
  	if(!(i % 4)) printf("\n");
  }*/
 // printf("length = %d\n",length);
  return eval(0,length-1);
    /* TODO: Insert codes to evaluate the expression. */
  

  
}

