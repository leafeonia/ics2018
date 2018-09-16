#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


char* test = "88 88 341809344 (64*67*(((66+(((65)))*7))))*(83/(57*74-((4+((23))))+18+(69/53)*36-12/96*(7-82)*(14))*(((86)+(0*(34)+24))/((((93))*(((47*5/((83)))))+(94/((47+6))+(((12))/73))+69))*((27))*66)+77+76) 82 (82) 79 79*(97/70) 97 97 25174 (53)+45/76+29+82*(34*9) 35 35 90 (90) 43 (43) 6 6 87 87 59 ((59)) 68 68 6398 25-(17)+(((80*((8))/((40/64/(((51)/((((21-(37)))))))+29+28*(10)-79-14+55/(72/((((((65))*76/15-10-62-92-(11)-88)-((32/24)))))*((82))/38*(37)*(42/(((78)-((((98/((9+(((14)))/(23/(((37)/((29)))))+(42)))-22+(41)-13/36/(((57))))/(((0+91/41-(78))))+91)))+(19)))+((8*16))-8-51+(2)-(1))*((48))+63-(98+(0))*52/(65)*(48-9)-25)*84*(44)+(((24)))-35*0+67*79+((69))-((((79-(76-(39)+10/((((91)/(9/(75)-9))))+(62)))+54)))+(33*(((((32/(23)/38/(66/37/(9)+35+92)))*(53)+29)/(27))*26))/(80)/(34)))))+((83))*75+83+78-((65)))+((69)) 4294967207 (((49))/((21)))/21-89 4294967207 ((((52/79)))+(((49)))-((((34+53*80/0*(24)-(27))/3/5+22+(76)+((44+14))-39))/19))*74-((68)) 95 (95) 95 ((56))*16-31*4+(16)-((((73)-((59-(37)+4)))))/81-((63)*((((((40/34-(23))*(63)+92+(((65/72)))+((((9))))+(30)-92*89-(((37))+((((63/92)+(((94+83))-(35)*(79*7*((((50/(42+((46)+(16+46/48*76+1)*52*((((31)))))/10+47*82*(73)+1)+(62)*22-24+47)))))))))*67)))))*57*(87)/(66/(87)/(1))/25))/25*(80)-37/((55+41-(48-((62)/((((((20/18+68+((95))-((20+62))/34))-81*1*(((70-17)*(10)))-78-((38)*57)-(72)-(7)-25/10*(((18/(88)))/7*((61-((15-93*82))-((85))-57+2)))+36+49+(61))))*54)))-37))*15-70+28 30 (30) 4294967228 14-82+(77)/(39+72)/43/23 35 35 13 (13) 0 (58/86) 1482 (57)*(30+(98)*61/((57))/((33)-99-79+31)/30-4) 39 39 52 52 89 89 60 60 0 74*(((57)/(22))/(97))*(89)/(12) 52 (52) 33 (33) 7189 (79)*91 12 12 4270 ((((61)))*70) 4294966729 93-(58-43)*(12+(32)) 4290151212 (73+((((69)+86/(70)*39+23+27))-(82)-((((65*74/(80)/59-38)*18*(((60-((56))/(52*(29/35*(((85+37)/70+67/((((((49)*65))-(23)/(((97)-61)))))-(41)*30+(74)-((10)*(((62)))*19)))+(36/((88)))+10/4+(0)*5+(25*11-28*(71)+((50-53))*(36)+((82+(97)*47-97/50)-(((30))))-78/96*(86)-(18)/65+((65+((50))*((((82))+99-(((82)))-98)*(92)-42))/(84-(((10)+((74))-66*5-((90))))*96*(54+71)-65*56-(((43/((39)))))*(4)-(9)*(2)*(35))-(31)/26*76+37/78)-((8/14-(84)/(28)-4))-51)*((((96)))/27*(33*(28))-8))*((((87-(73)-74-(15)))))-((5))+50-76))-((74-83+(((48/(((62)))-88*46*85))))))))*((93))))/33)) 0 (7/11) 2584 ((((((11)*((89/34))+9)+(16)))*(((55))))-(32)/19) 4826 61*78+(98)-30";





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

/*static int cmd_p(char *args){
	bool* success = (bool*)malloc(sizeof(bool));
	*success = true;
	uint32_t ans = expr(args,success);
	if(*success) printf("%u\n",ans);
	else printf("Invalid expression\n");
	free(success);
	return 0;
}*/

static int cmd_p(char *args){
	bool* success = (bool*)malloc(sizeof(bool));
	*success = true;
	int correct;
	char* exp = (char*)malloc(sizeof(char));
	sscanf(test,"%d",&correct);
	sscanf(test,"%s",exp);
	//char* arg = strtok(test," ");
	
	printf("first arg = %s\n",exp);
	/*int i = 0;
	  while(arg){
		printf("arg=%s\n",arg);
	  	printf("test%d = %u\n",i++,cmd_p(strtok(NULL," ")));
	  	arg = strtok(NULL," ");
	  }*/
	//uint32_t ans = expr(args,success);
	//if(*success) printf("%u\n",ans);
	//else printf("Invalid expression\n");
	free(success);
	printf("success\n");
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

/*int main(int argc,char* argv[]){
  char* arg = strtok(argv," ");
  while(arg){
    printf("arg=%s\n",arg);
  	printf("test = %u\n",cmd_p(arg));
  	arg = strtok(NULL," ");
  	arg = strtok(NULL," ");
  }
  return 0;
}*/
