#include "procedure.h"
#include<stdio.h>

void Produci(struct ProdCons * pc, int valore){

    enter_monitor(&(pc->m));

    printf("Ingresso monitor -produzione\n");

    while(pc->buffer_occupato==1){
        printf("Sospensione - produzione\n");
        wait_condition(&(pc->m),VARCOND_PRODUTTORI);
        printf("Riattivazione -produzione\n");
    }

    pc->buffer=valore;
    pc->buffer_libero=0;
    pc->buffer_occupato=1;

    printf("Produzione(%d)\n", valore);
}