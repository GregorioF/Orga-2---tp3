/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"

int tareas [8]={1,1,1,1,1,1,1,1};
int banderas [8]={1,1,1,1,1,1,1,1};
int current = -1;
int currentBanderas = -1;

void sched_inicializar() {
}

void inhabilitar_tarea(unsigned int error ,short n){
	if (n != -1){
		tareas[n]=0;
		banderas[n]=0;
		borrarDelMapa(n);
		printearError(n,error);
	}
}
short sched_proximo_indice() {
	current +=1 ;
	int i = 0;
	while(tareas[current%8] == 0 && i < 20){
		current +=1;
		i = i+1;
	}
	if(i == 20) return -1;
	return current %8;
}

short sched_indice_actual(){
	if ( 7 < current ){
		current = current%8;
	};
	return current 	;
}
short sched_proxima_bandera(){
	currentBanderas +=1;

	while(tareas[currentBanderas] == 0 ){
		currentBanderas +=1;
		if(currentBanderas == 8) {
			currentBanderas = -1;
			break;
		}
	}
	if( 7 < currentBanderas ) currentBanderas = -1;
	return currentBanderas;
}

int* tareas_arreglo (){
	return tareas;
}
short sched_bandera_actual(){
	if ( 7 < currentBanderas){
		currentBanderas = currentBanderas%8;
	}
	return currentBanderas;
}

