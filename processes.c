#include "inttypes.h"
#include "processes.h"
#include "cpu.h"
#include "string.h"
#include "stdio.h"

struct process processes[2];

void idle(){
  printf("processus idle, je tente de passer la main à proc1 \n");


  ctx_sw(processes[0].save_zone,  processes[1].save_zone);
}

void proc1(){
  printf("[proc1] idle a donne la main \n");
  printf("sais-tu que %d + %d = %d ? \n", 1, 1, 2);
  printf("[proc1] j’arrete le systeme \n");
  hlt();
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
  processes[1].state = ELIGIBLE;

  processes[1].save_zone[1] = (uint32_t)(processes[1].reg+STACK_LENGTH-1);
  processes[1].reg[511] = (uint32_t)&proc1;
}
