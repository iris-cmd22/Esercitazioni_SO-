#include "header.h"
#include <stdio.h>

void Produci(struct Prodcons * pc, int value){

        enter_monitor(&(pc->m));

        printf("Ingresso monitor - produzione\n");

        if(pc->buffer_occupato==1){

            printf("Sospensione-produzione\n");
            wait_condition(&(pc->m),VARCOND_PRODUTTORI);
            printf("Riattivazione - produzione\n");
        }

        pc->buffer=value;
        pc->buffer_occupato=1;
        pc->buffer_libero=0;

        printf("Produzione (%d)\n",value);
        //Adesso sveglio i consumatori perchè ho prodotto
        signal_condition(&(pc->m), VARCOND_CONSUMATORI);

        leave_monitor(&(pc->m));

        printf("Uscita monitor - produzione\n");
}


int Consuma(struct Prodcons * pc){
    int value;

    enter_monitor(&(pc->m));
    printf("Ingresso monitor - consumazione\n");

    if(pc->buffer_libero==1){
        printf("Sospensione-consumazione\n");
        wait_condition(&(pc->m),VARCOND_CONSUMATORI);
        printf("Riattivazione-consumazione\n");
    }

    value=pc->buffer;
    pc->buffer_libero=1;
    pc->buffer_occupato=0;

    printf("Consumazione (%d)\n");
    signal_condition(&(pc->m),VARCOND_PRODUTTORI);
    leave_monitor(&(pc->m));
    return value;
}