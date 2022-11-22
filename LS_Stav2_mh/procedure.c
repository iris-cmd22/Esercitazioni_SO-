#include"header.h"

#include<unistd.h>
#include<stdio.h>

int Leggi(struct LettScritt* ls){

    int valore;
    enter_monitor(&(ls->m));

    printf("Lettura - ingresso monitor\n");

    if(ls->numero_scrittori>0){

        printf("Sospensione-lettura\n");
        wait_condition(&(ls->m),VARCOND_LETTORI);
        printf("Riattivazione-lettura\n");
    }

    ls->numero_lettori++;
    printf("Numero lettori++: %d\n", ls->numero_lettori);

    signal_condition(&(ls->m),VARCOND_LETTORI);

    leave_monitor(&(ls->m));

    sleep(2);

    valore=ls->buffer;
    printf("Lettura-valore[%d]\n",valore);

    enter_monitor(&(ls->m));

    ls->numero_lettori--;

    printf("Numero lettori -- : %d\n", ls->numero_lettori);

    //se sono l'ultimo lettore devo liberare gli scrittori
    if(ls->numero_lettori ==0){
        printf("Lettura -signal su scrittori\n");
        signal_condition(&(ls->m),VARCOND_SCRITTORI);
    }

    leave_monitor(&(ls->m));

    printf("Lettura - uscita monitor\n");

    return valore;
}

void Scrivi(struct LettScritt * ls, int valore){

    enter_monitor(&(ls->m));
    printf("Scrittura - ingresso monitor\n");

    if(ls->numero_lettori > 0 || ls->numero_scrittori > 0){

        printf("Scrittura - ingresso monitor\n");
        wait_condition(&(ls->m),VARCOND_SCRITTORI);
        printf("Scrittura - riattivazione\n");
    }

    ls->numero_scrittori++;

    printf("Numero scrittori ++: %d\n", ls->numero_scrittori);

    leave_monitor(&(ls->m));

    //...operazione lenta...
    sleep(1);

    ls->buffer=valore;

    printf("Scrittura - valore[%d\n", ls->numero_scrittori);


    enter_monitor(&(ls->m));

    ls->numero_scrittori--;

    printf("Numero scrittori --: %d\n", ls->numero_scrittori);

    if(queue_condition(&(ls->m),VARCOND_SCRITTORI)){
        printf("Scrittura -signal su scrittori\n");
        signal_condition(&(ls->m), VARCOND_SCRITTORI);
    }else{

        printf("Scrittura - signal su lettori\n");
        signal_condition(&(ls->m), VARCOND_LETTORI);
    }

    leave_monitor(&(ls->m));

    printf("Scrittura - uscita monitor\n");

}