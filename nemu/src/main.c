#include "monitor/expr.h"
#include <stdlib.h>
#include <sys/types.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  bool *success = (bool*)malloc(sizeof(bool));
  printf("test:%u\n",expr("1+2",success));
  free(success);
  //the code for expression evaluation correctness examination is in ui.c(required by PA1.2)
  
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
