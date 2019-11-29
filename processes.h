#include "inttypes.h"
// la taille de la pile d'éxécution des processus
#define STACK_LENGTH 512
// nombre de processus qui seront exécutés dans le programme.
#define p_number 2

// defini l'état des processus.
typedef enum proccess_state process_state;
enum proccess_state {
    ELU,
    ACTIVABLE,
    ENDORMI
    };

struct process {
  uint32_t pid;
  char name[32];
  process_state state;
  int wake_time;
  struct process *next;
  uint32_t save_zone[5];

  uint32_t reg[STACK_LENGTH];
  };

//void *context_idle, *context_proc1;
void idle();
void proc1();
uint16_t mon_pid();
char *mon_nom();
void ordonnance();
uint32_t create_process(char *name, void *proc_address);
void init_processes();
void ctx_sw(void*, void*);
void remove_sleep_head();
void remove_process(struct process *p);
void add_process(struct process *p);
void add_in_sleep (struct process *p);
void wake_up_processes();
struct process *get_next();
void dors(uint32_t nbr_secs);
int nbr_secondes();
