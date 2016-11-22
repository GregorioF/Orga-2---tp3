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
	if (0 <= n){
		banderas[n]=0;
		tareas[n]=0;
		borrarDelMapa(n);
		printearError(n,error);
	}
}
short sched_proximo_indice() {
	if (current < 7){
		current +=1 ;
	}
	else{
		current = 0;
	}
	
	while(tareas[current%8] == 0){
		current +=1;
		if(16 <= current){
			current = -1;
			return current;
		}
	}
	current = current%8;
	return current;
}

short sched_indice_actual(){
	return current;
}
short sched_proxima_bandera(){
	if (currentBanderas == 7){
		currentBanderas = -1;
		return currentBanderas;
	}
	currentBanderas +=1;
	
	while(tareas[currentBanderas%8] == 0 ){
		currentBanderas +=1;
		if(8 == currentBanderas) {
			currentBanderas = -1;
			return currentBanderas;
		}
	}
	return currentBanderas%8;
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

