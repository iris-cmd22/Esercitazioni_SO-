#ifndef HEADER_H
#define HEADER_H

#include "monitor_signal_continue.h"

#define DIM 0

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

struct ProdCons
{
    int buffer[DIM];
    int stato[DIM];
    
    int numero_liberi;
    int numero_occupati;

    Monitor m;
};

#define VARCOND_PRODUTTORI 0
#define VARCOND_CONSUMATORI 1

void Produci(struct ProdCons *pc, int valore);
int Consuma(struct ProdCons * pc);



#endif