#include "inttypes.h"
#include "processes.h"
#include "malloc.c.h"
#include "cpu.h"
#include "string.h"
#include "stdio.h"

struct process **processes;
struct process *running;
int N;
uint32_t processes_created = 0;
void idle(){
  for(;;){
    printf("bienvenue chez idle \n");
    printf("------------------------ \n");
    printf("processus idle, je tente de passer la main au proc1 \n");
    printf("mon pid est : %d\n", mon_pid());
    ordonnance();
    //ctx_sw(processes[0].save_zone,  processes[1].save_zone);
  }

}

void proc1(){
  for(;;){
    printf("bienvenue chez proc1 \n");
    printf("------------------------ \n");

    printf("[proc1] a pris la pris la main \n");
    printf("processus proc1 de pid %d !", mon_pid());
    ordonnance();
  }
}

void proc2(){
  for(;;){
    printf("bienvenue chez proc2 \n");
    printf("------------------------ \n");

    printf("[proc1] a pris la pris la main \n");
    printf("processus proc1 de pid %d !", mon_pid());
    ordonnance();
  }
}

uint16_t mon_pid(){
   return running->pid;
}

void ordonnance(){

  if(running == processes[0]){
      running = processes[1];
      processes[0]->state = ACTIVABLE;
      processes[1]->state = ELU;
      ctx_sw(processes[0]->save_zone, processes[1]->save_zone);

  }
  else if(running == processes[1]){
    running = processes[0];
    processes[1]->state = ACTIVABLE;
    processes[0]->state = ELU;
    ctx_sw(processes[1]->save_zone, processes[0]->save_zone);
  }
}

// cette fonction permet de créer les processus dynamiquement
uint32_t create_process(char *name, void *proc_address){

  if(processes_created >= N){
    printf("you processess' stack is full ! cannot create mote !!!\n");
    return -1;
  }
  struct process *p = malloc(sizeof(struct process));
  p->pid  = processes_created;
  strcpy(p->name, name);
  p->state = ACTIVABLE;
  p->save_zone[1] = (uint32_t)(p->reg+STACK_LENGTH-1);
  p->reg[511] = (uint32_t )proc_address;
  processes[processes_created] = p;
  
  processes_created ++;
  return processes_created - 1 ;

}
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display
void init_processes(int n){
  N = n;
  processes = malloc(N*sizeof( struct process *));
  // tableau contenant la structure des 2 processus
  processes[0] = malloc(sizeof(struct process ));
  processes[0]->pid = 0;
  strcpy(processes[0]->name, "idle");
  processes[0]->state = ELU;
  processes_created++;
  
  if(create_process("proc1", &proc1) == -1 ){
    printf("the process proc1 cannot be created \n");
    return ;
  }
  /*processes[1]->pid = 1;
  strcpy(processes[1]->name, "proc1");
  processes[1]->state = ACTIVABLE;

  processes[1]->save_zone[1] = (uint32_t)(processes[1]->reg+STACK_LENGTH-1);
  processes[1]->reg[511] = (uint32_t)&proc1;*/
  running = processes[0];
  idle();
}
