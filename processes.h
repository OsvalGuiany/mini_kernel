#include "inttypes.h"
// la taille de la pile d'éxécution des processus
#define STACK_LENGTH 512
// nombre de processus qui seront exécutés dans le programme.
#define p_number 2

// defini l'état des processus.
typedef enum proccess_state process_state;
enum proccess_state {
    ELU,
    ACTIVABLE
    };

struct process {
  uint16_t pid;
  char name[32];
  process_state state;
  uint32_t save_zone[5];
  uint32_t reg[STACK_LENGTH];
  };

//void *context_idle, *context_proc1;
void idle();
void proc1();
uint16_t mon_pid();
void ordonnance();
void init_processes();
void ctx_sw(void*, void*);
