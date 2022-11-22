#include "header.h"

#include<unistd.h>
#include<stdio.h>

void Produci(struct ProdCons * pc, int valore){

    enter_monitor(&(pc->m));

    printf("Ingresso monitor - produzione\n");

    while(pc->numero_liberi==0){
        printf("Sospensione - produzione\n");
        wait_condition(&(pc->m), VARCOND_PRODUTTORI);
        printf("Riattivazione-consumazione\n");
    }

    int i=0;
    while(i<DIM && pc->stato[i] !=LIBERO){
        i++;
    }

    pc->stato[i]=IN_USO;
    pc->numero_liberi--;

    leave_monitor(&(pc->m));

    //metto una sleep per simulare una produzione lenta
    sleep(2);

    pc->buffer[i]=valore;

    printf("Produzione - posizione %d, valore %d\n", i, valore);


    enter_monitor(&(pc->m));

    pc->stato[i]=OCCUPATO;
    pc->numero_occupati++;

    signal_condition(&(pc->m), VARCOND_CONSUMATORI);

    printf("Uscita monitor -produzione\n");

    leave_monitor(&(pc->m));
}

int Consuma(struct ProdCons *pc){
    int valore;

    enter_monitor(&(pc->m));
    printf("Ingresso monitor-consumazione\n");

    while(pc->numero_occupati==0){
        printf("Sospensione - consumazione\n");
        wait_condition(&(pc->m),VARCOND_CONSUMATORI);
        printf("Riattivazione-consumazione\n");
    }

    int i=0;
    while(i<DIM && pc->stato[i]!=OCCUPATO){
        i++;
    }

    pc->stato[i]=IN_USO;
    pc->numero_occupati--;

    leave_monitor(&(pc->m));

    //sleep per rallentare l'esecuzione
    sleep(2);

    valore=pc->buffer[i];

    printf("Consumazione - posizione %d, valore%d\n",i,valore);

    enter_monitor(&(pc->m));

    pc->stato[i]=LIBERO;
    pc->numero_liberi++;

    signal_condition(&(pc->m),VARCOND_PRODUTTORI);
    leave_monitor(&(pc->m));

    printf("Uscita monitor - consumazione\n");

    return valore;
}