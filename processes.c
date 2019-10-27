#include "inttypes.h"
#include "processes.h"
#include "cpu.h"
#include "string.h"
#include "stdio.h"

struct process processes[2];
struct process *running;
int n_idle = 0;
int n_proc = 0;

void idle(){
  for(;;){
    printf("bienvenue chez idle \n");
    printf("------------------------ \n");
    printf("processus idle, je tente de passer la main au proc1 \n");
    printf("mon pid est : %d, n_idle est %d hop !\n", mon_pid(), n_idle);
    ordonnance();
    //ctx_sw(processes[0].save_zone,  processes[1].save_zone);
  }

}

void proc1(){
  for(;;){
    printf("bienvenue chez proc1 \n");
    printf("------------------------ \n");
    printf("[proc1] idle a donne la main \n");
    printf("processus proc1 de pid %d , n_proc est %d et hop ! \n", mon_pid(), n_proc);
    ordonnance();
  }
}

uint16_t mon_pid(){
   return running->pid;
}

void ordonnance(){

  if(running == processes){
      running = processes + 1;
      n_proc ++;
      processes[0].state = ACTIVABLE;
      processes[1].state = ELU;
      ctx_sw(processes[0].save_zone, processes[1].save_zone);

  }
  else if(running == processes + 1){
    running = processes;
    n_idle++;
    processes[1].state = ACTIVABLE;
    processes[0].state = ELU;
    ctx_sw(processes[1].save_zone, processes[0].save_zone);
  }
}

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display
void init_processes(){
  // tableau contenant la structure des 2 processus
  processes[0].pid = 0;
  strcpy(processes[0].name, "idle");
  processes[0].state = ELU;

  processes[1].pid = 1;
  strcpy(processes[1].name, "proc1");
  processes[1].state = ACTIVABLE;

  processes[1].save_zone[1] = (uint32_t)(processes[1].reg+STACK_LENGTH-1);
  processes[1].reg[511] = (uint32_t)&proc1;
  running = processes;
  idle();
}
