#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if  (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  } 

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);


static int cmd_x(char *args){
	char* arg1 = strtok(args," ");
	char* arg2 = strtok(NULL," ");
	if(arg2) {
		int length = atoi(arg1);
		int address;
		sscanf(arg2,"%x",&address);
		for(int i = 0;i < length;++i){
			printf("0x%06x:0x%08x\n",address,vaddr_read(address,4));
			address+= 4;
	 	}
	 }
	else if(arg1){
		int address; 
		sscanf(arg1,"%x",&address);  
		printf("0x%06x:0x%08x\n",address,vaddr_read(address,4));  
	}
	return 0;
}

static int cmd_info(char *args){
	char* arg1 = strtok(args," ");
 	if(arg1 && arg1[0] == 'r'){
		char* list[] = {"eax","edx","ecx","ebx","ebp","esi","edi","esp"};
	 	for(int i = 0;i < 8; ++i){
			printf("%s : %08x      ",list[i],cpu.gpr[i]._32);
			if((i+1) % 4 == 0) printf("\n");
		}
		printf("eip : %08x\n",cpu.eip); 
	} 
	return 0;
}

static int cmd_s(char *args){
	char* arg1 = strtok(args," ");
	int length = 1;
	if(arg1) length = atoi(arg1);
	cpu_exec(length);
	return 0;
}

static int cmd_p(char *args){
	char* arg1 = strtok(args," ");
	bool* success = (bool*)malloc(sizeof(bool));
	*success = true;
	uint32_t ans = expr(arg1,success);
	if(*success) printf("%d\n",ans);
	else printf("Invalid expression");
	free(success);
	return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "x", "Memory scan",cmd_x},
  { "info", "Register/Watchpoint information",cmd_info},
  { "s", "execute N instructions",cmd_s},
  { "p", "calculate the value of the given expression", cmd_p},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
