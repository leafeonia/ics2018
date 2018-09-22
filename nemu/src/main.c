#include "monitor/expr.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  bool *success = (bool*)malloc(sizeof(bool));
  char exp[10000];
  int correct;
  FILE* f = fopen("./tools/gen-expr/input","r");
  for(int i = 0;i < 100;++i){
  	memset(exp,0,sizeof(exp));
  	fscanf(f,"%d %s",&correct,exp);
  	printf("%d %u\n",correct,expr(exp,success));
  }
  free(success);
  //the code for expression evaluation correctness examination is in ui.c(required by PA1.2)
  
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
