#include "inttypes.h"
#include "cpu.h"
#include "ecran.h"
#include "stdio.h"
#include "stdbool.h"
#include "segment.h"
#include "time_print.h"

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

static int compteur = 0;


void tic_PIT() {
  compteur++;
  char ch[9];

  outb(0x20, 0x20);
  snprintf(ch, 9, "%02d:%02d:%02d", (compteur/60)/60, (compteur/60)%60, compteur%60);
  masque_IRQ(0, false);
  print_right(ch, 8);

}

void set_timer(){
  outb(0x34, 0x43);
  outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
}

void init_traitant_IT(int32_t num_IT, void (*traitant)(void)) {
    uint32_t *vect_int = (uint32_t *)((void *)0x1000 + 32*8);

    // premier mot
    *vect_int = (KERNEL_CS<<16) + ((uint32_t)traitant & 0x0000ffff);
    //deuxième mot
    // autre notation possible *(vect_int+1)
    *(uint32_t *)((void *)vect_int+4) = ((uint32_t)traitant&0xffff0000) + 0x8E00;
}

void masque_IRQ(uint32_t num_IRQ, bool masque) {
    uint32_t val_act = inb(0x21);
    uint32_t operateur = 0;
    /* if (masque) {
      val_act = val_act | (0b1<<num_IRQ);
    }
    else{
       // permet de récuếrer les bits de 7 à num_IRQ
       uint32_t pre_IRQ = val_act>>(num_IRQ+1);

       uint32_t post_IRQ = (val_act&)>>(8-num_IRQ);
       val_act = post_IRQ | (masque<<num_IRQ) | (pre_IRQ<<(num_IRQ+1));
    }*/
    if (masque) {
      for(int i = 7; i>=0; i--){
        if(i == num_IRQ){
          operateur = (operateur<<1)+masque;
        }
        else{
          operateur = (operateur<<1)+0b0;
        }
      }
      val_act = val_act|operateur;
    }
    else {
      for(int i = 7; i>=0; i--){
        if(i == num_IRQ){
          operateur = (operateur<<1)+0b0;
        }
        else{
          operateur = (operateur<<1)+0b1;
        }
      }
      val_act = val_act&operateur;
    }

    outb(val_act, 0x21);
}
