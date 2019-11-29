#include "inttypes.h"
#include "stdbool.h"

void tic_PIT();
void init_traitant_IT(int32_t num_IT, void (*traitant)(void));
void masque_IRQ(uint32_t num_IRQ, bool masque);
void set_timer();
void traitant_IT_32();
int get_compteur();
