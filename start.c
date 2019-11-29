#include "cpu.h"
#include "inttypes.h"
#include "ecran.h"
#include "string.h"
#include "time_print.h"
#include "processes.h"


void kernel_start(void)
{
    /*set_timer();
    masque_IRQ(0, 0);
    init_traitant_IT(32, traitant_IT_32);

    place_curseur(0, 0);
    efface_ecran();
    printf("bonjour a l'ENSIMAG \n");
    printf("Bonjour \n");
    printf("Manufacturer, Mercury ... \n");
    // on ne doit jamais sortir de kernel_start
    sti();
    */
    
    
    set_timer();
    masque_IRQ(0,0);
    init_traitant_IT(32, traitant_IT_32);

    efface_ecran();
    init_processes(8);

    while (1) {
        // cette fonction arrete le processeur
        hlt();
    } 
}
