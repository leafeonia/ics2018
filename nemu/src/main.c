int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /*bool *success = (bool *)malloc(sizeof(bool));
  char* arg = strtok(argv," ");
  while(arg){
  	printf("test = %u\n",expr(arg,success));
  	arg = strtok(NULL," ");
  	arg = strtok(NULL," ");
  }
  free(success);*/

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
