#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_NUM = 0,
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
  {"\\d+",TK_NUM},
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

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case 0:
    		tokens[i].type = 0;
    		for(int j = 0;j < substr_len;++j){
    			tokens[i].str[j] = substr_start[j];
    		}
    		break; 
    	  case 1:
    		tokens[i].type = 1;
    		break; 
    	  case 2:
    		tokens[i].type = 2;
    		break; 
    	  case 3:
    		tokens[i].type = 3;
    		break; 
    	  case 4:
    		tokens[i].type = 4;
    		break; 
    	  case 5:
    		tokens[i].type = 5;
    		break; 	
    	  case 6:
    		tokens[i].type = 6;
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

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  printf("%p\n",(int*)tokens);
  /* TODO: Insert codes to evaluate the expression. */
  

  return 0;
}
