#include "cpu.h"
#include <string.h>
#include "inttypes.h"

#define start_p 0xB8000;

static uint32_t lig_curseur = 0;
static uint32_t col_curseur = 0;

void avance_curseur();

uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
   uint16_t *start;
   start = (uint16_t *)(0xB8000 + 2*(lig*80+col));

   return start;
}

void place_curseur(uint32_t lig, uint32_t col) {
  uint16_t mem = (uint16_t)(lig*80+col);

  if (lig<25 && col<80) {
    outb(0x0F, 0x3D4);
    outb(mem&0xff, 0x3D5);
    outb(0x0E, 0x3D4);
    outb(mem>>8, 0x3D5);

    lig_curseur = lig;
    col_curseur = col;
  }
}

void ecrit_car(uint32_t lig, uint32_t col, char c) {
   uint8_t *mem_car = (uint8_t *)ptr_mem(lig, col);
  *mem_car = c;
  *(mem_car+1) = (0<<7) + (0x0<<4) + 0xf;
}

void efface_ecran(){
    for (uint32_t i = 0; i<80; i++){
      for (uint32_t j = 0; j<25; j++){
        *ptr_mem(j, i) = (uint8_t)0x0720;
      }
    }
    lig_curseur = 0;
    col_curseur = 0;
}

void traite_car(char c){
  if(strcmp(&c, "\b") == 0){
     if(col_curseur>0){
       col_curseur -=1 ;
       place_curseur(lig_curseur, col_curseur);
     }
  }
  else if(strcmp(&c, "\t") == 0){
     uint32_t tabul = (col_curseur/8)*8+8;

     if(tabul > 79){
       tabul = 79;
     }

     col_curseur = tabul;
     place_curseur(lig_curseur, col_curseur);
  }
  else if(strcmp(&c, "\n") == 0){
    col_curseur = 0;
    lig_curseur += 1;
    place_curseur(lig_curseur, 0);

  }
  else if (strcmp(&c, "\f") == 0){
     efface_ecran();
     col_curseur = 0;
     lig_curseur = 0;
     place_curseur(col_curseur, lig_curseur);
  }
  else if(strcmp(&c, "\r") == 0){
    col_curseur = 0 ;
     place_curseur(lig_curseur, 0);
  }
  else{
     ecrit_car(lig_curseur, col_curseur, c);
     avance_curseur();
  }
}

void defilement(){
  for(uint32_t i = 1; i<25; i++){
    for(uint32_t j = 0; j<80; j++){
      memmove( ptr_mem(i-1, j), ptr_mem(i, j), 2);
    }
  }
  for(uint32_t j=0; j<80; j++){
    ecrit_car(24, j, 32);
  }
}

void avance_curseur(){
  if(lig_curseur == 24){
    if(col_curseur == 79){
      defilement();
      col_curseur = 0;
    }
    else{
      col_curseur +=1;
    }

  }

  else if (col_curseur == 79){
    if(lig_curseur<24){
      lig_curseur += 1;
    }
    else{
      defilement();
    }
    col_curseur = 0;

  }
  else{
    col_curseur += 1;
  }
  place_curseur(lig_curseur, col_curseur);
}

void console_putbytes(char *chaine, uint32_t taille){
  for(uint32_t i = 0; i<taille; i++){

    traite_car(chaine[i]);
  }
}

void print_right(char *c, int num) {
  uint32_t initial = 80-num;
   uint32_t lig = lig_curseur;
   uint32_t col = col_curseur;
   for (int i=0; i<num; i++) {
      ecrit_car(0, initial+i, c[i]);
   }
   lig_curseur= lig;
   col_curseur = col;
   place_curseur(lig_curseur, col_curseur);
}
