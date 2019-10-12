#include <stdio.h>
#include "inttypes.h"

uint16_t *ptr_mem(uint32_t, uint32_t);
void ecrit_car(uint32_t, uint32_t, char);
void efface_ecran();
void place_curseur(uint32_t, uint32_t);
void traite_car(char c);
void console_putbytes(char *, uint32_t);
void print_right(char *, int);
void defilement();
