#include "cpu.h"
#include <inttypes.h>
#include "ecran.h"
#include "time_print.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    set_timer();
    masque_IRQ(0, 0);
    init_traitant_IT(32, traitant_IT_32);

    place_curseur(0, 0);
    efface_ecran();
    printf("bonjour a l'ENSIMAG \n");
    printf("Bonjour \n");
    printf("Manufacturer, Mercury ... \n");
    // on ne doit jamais sortir de kernel_start
    sti();
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
