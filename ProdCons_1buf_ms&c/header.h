#ifndef HEADER_H
#define HEADER_H

#include "monitor_hoare.h"

struct Prodcons {
        int buffer;

        //variabili di appoggio
        int buffer_libero;
        int buffer_occupato;
        Monitor m;
};

//MACRO per varcondition
#define VARCOND_PRODUTTORI 0
#define VARCOND_CONSUMATORI 1

void Produci(struct Prodcons * pc, int value);
int Consuma(struct Prodcons * pc);

#endif