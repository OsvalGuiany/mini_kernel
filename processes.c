#include "inttypes.h"
#include "processes.h"
#include "malloc.c.h"
#include "cpu.h"
#include "string.h"
#include "stdio.h"
#include "time_print.h"

struct process **processes;
struct process *running;
int N;
uint32_t processes_created = 0;
struct process *head, *last;
struct process *sleep_head, *sleep_last;

void idle(){
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    sti();
    hlt();
    cli();
  }
}

void proc1(){
  for (;;) {
    printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
    mon_nom(), mon_pid());
    dors(2);
  }
}

void proc2(){
  for (;;) {
    printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
    mon_nom(), mon_pid());
    dors(3);
  }
}

void proc3(){
  for (;;) {
    printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
    mon_nom(), mon_pid());
    dors(5);
  }
}

void proc4(){
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    sti();
    hlt();
    cli();
  }
}

void proc5(){
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    sti();
    hlt();
    cli();
  }
}

void proc6(){
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    sti();
    hlt();
    cli();
  }
}

void proc7(){
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    sti();
    hlt();
    cli();
  }
}

uint16_t mon_pid(){
   return running->pid;

}

char *mon_nom() {
  return (char *)&(running->name);
}

void ordonnance(){

  struct process *next ;
  struct process *prev;

  next = get_next();

  next ->state = ELU;

  if(running->state == ENDORMI)
    add_in_sleep(running);
  else
  {
    add_process(running);
  }

  prev = running;
  running = next;
  wake_up_processes();
  ctx_sw(prev->save_zone, next->save_zone);
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
  p->wake_time = 0;
  p->next = NULL;
  p->save_zone[1] = (uint32_t)(p->reg+STACK_LENGTH-1);
  p->reg[511] = (uint32_t )proc_address;
  add_process(p);
  processes[processes_created] = p;
  
  processes_created ++;
  return processes_created - 1 ;

}
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display
void init_processes(int n){
  N = n;
  head = NULL;
  last = NULL;
  processes = malloc(N*sizeof(struct process *));
  // tableau contenant la structure des 2 processus
  processes[0] = malloc(sizeof(struct process ));
  processes[0]->pid = 0;
  strcpy(processes[0]->name, "idle");
  processes[0]->state = ELU;
  processes_created++;
  
  if(create_process("proc1", &proc1) == -1){
    printf("the process proc1 cannot be created \n");
    return ;
  }

  if(create_process("proc2", &proc2) == -1 ){
    printf("the process proc2 cannot be  if (p->state == ENDORMI ) { created \n");
    return ;
  }

  if(create_process("proc3", &proc3) == -1 ){
    printf("the process proc3 cannot be created \n");
    return ;
  }

  if(create_process("proc4", &proc4) == -1 ){
    printf("the process proc4 cannot be created \n");
    return ;
  }
  if(create_process("proc5", &proc5) == -1 ){
    printf("the process proc5 cannot be created \n");
    return ;
  }
  if(create_process("proc6", &proc6) == -1 ){
    printf("the process proc6 cannot be created \n");
    return ;
  }

  if(create_process("proc7", &proc7) == -1 ){
    printf("the process proc7 cannot be created \n");
    return ;
  }

  running = processes[0];
  idle();
}

void add_process(struct process *p){

  p->next = NULL;

  if(p->state == ENDORMI) {
    add_in_sleep(p);
    return;
  }

  p->state = ACTIVABLE;

  if(head == NULL ) {
    head = p;
    last = head;
  }
  else{
    last->next = p;
    last = p;
  }
  
}

void add_in_sleep (struct process *p) {
  printf("adding %d with time %d in sleep and current %d\n", 
     p->pid, p->wake_time, get_compteur());

  p->state = ENDORMI;
  p->next = NULL;

  if(sleep_head == NULL ) {
    sleep_head = p;
    sleep_last = p;
  } else {
    struct process *jet, *prev;
     jet = sleep_head;

    while ((jet != NULL) && (jet->wake_time <= p->wake_time)) {
      prev = jet;
      jet = jet->next;
    }

    p->next = jet;

    if(prev != NULL ) {
      prev->next = p;
    } else if (jet == NULL ) {
      prev->next = p;
      sleep_last = p;
    }else {
      sleep_head = p;
    }
  
  }
}

void remove_sleep_head() {
  struct process *p = sleep_head;

  if( p == sleep_last ) {
    sleep_head = NULL;
    sleep_last = NULL;
  }
  else{
    sleep_head = sleep_head->next;
  }
  p->wake_time = 0;
  p->next = NULL;

  p->state = ACTIVABLE;
  add_process(p);
}

void wake_up_processes(){

  while (sleep_head != NULL && sleep_head->wake_time <= get_compteur()) {

    printf("waking up of process %d with sleep time %d \n", 
      sleep_head->pid, sleep_head->wake_time);

    remove_sleep_head();
  }
}

struct process *get_next(){

    if(head == NULL ){
      add_process(processes[1]);
      add_process(processes[0]);
    }

    struct process *p = head;
    head = head->next;
    p->next = NULL;

    return p;
}

void dors(uint32_t nbr_secs) {
   int wake_time = nbr_secs*60 + get_compteur();
   running->wake_time = wake_time;
   running->state = ENDORMI ;
   ordonnance();
}

int nbr_secondes(){
  return get_compteur()/60;
}