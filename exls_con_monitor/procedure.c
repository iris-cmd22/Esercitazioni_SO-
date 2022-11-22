#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void InizioLettura(Monitor* m, Buffer * buf){
	
	int value;
	/* TBD: Effettuare inizio lettura */
	enter_monitor(&(buf->m));

	printf("Lettura - ingresso monitor\n");

	//Verifico che M non stia scrivendo
	if(buf->numero_scrittori>0){

		printf("Sospendo lettura\n");
		wait_condition(m,SYNCHL);
		printf("Riattivo lettura\n");
	}

	buf->numero_lettori++;

	leave_monitor(&(buf->m));

}

void FineLettura(Monitor* m, Buffer * buf) {

	/* TBD: Effettuare fine lettura */
	enter_monitor(&(buf->m));

	printf("Lettura - ingresso monitor\n");

	buf->numero_lettori--;

	//se sono l'ultimo lettore libero lo scrittore
	if(buf->numero_lettori==0){

		printf("Lettura-Libero scrittori\n");
		signal_condition(m,SYNCHS);
		printf("Termino lettura\n");
	}

	leave_monitor(&(buf->m));

}



//Procedure di inizio e fine scrittura

void InizioScrittura(Monitor* m,Buffer*buf){

	/* TBD: Effettuare inizio scrittura */
	enter_monitor(&(buf->m));

	printf("Scrittura - ingresso monitor\n");

	//Verifico che M non stia scrivendo
	if(buf->numero_lettori>0){

		printf("Sospendo scrittura\n");
		wait_condition(m,SYNCHS);
		printf("Riattivo scrittura\n");
	}

	buf->numero_scrittori++;

	leave_monitor(&(buf->m));
}

void FineScrittura(Monitor* m, Buffer*buf){

	/* TBD: Effettuare inizio scrittura */
	enter_monitor(&(buf->m));

	printf("Scrittura - ingresso monitor\n");

	buf->numero_scrittori--;

	if(buf->numero_scrittori==0){

		printf("Scrittura - Libero lettori\n");
		signal_condition(m,SYNCHL);
		printf("Termino scrittura\n");
	}

	leave_monitor(&(buf->m));
}



void Servizio(Monitor* m, Buffer * buf){

	srand(time(0));

	int i;
	for(i=0; i<20; i++) {

		/* TBD: Richiamare InizioScrittura e FineScrittura */
		InizioScrittura(&m,&buf);

		buf->meteo.temperatura = (rand()%101)-50;
		buf->meteo.umidita = rand()%101;
		buf->meteo.pioggia = rand()%2;

		printf("<%d> scrittura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), buf->meteo.temperatura, buf->meteo.umidita, (buf->meteo.pioggia ? "si" : "no") );

		sleep(2);

		FineScrittura(&m,&buf);
	}

}

void Utente(Monitor* m, Buffer * buf) {

	int i;
	for(i=0; i<10; i++) {
		
		/* TBD: Richiamare InizioLettura e FineLettura */
		InizioLettura(&m,&buf);

		printf("<%d> lettura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), buf->meteo.temperatura, buf->meteo.umidita, (buf->meteo.pioggia ? "si" : "no") );

		sleep(1);

		FineLettura(&m,&buf);
	}

}


