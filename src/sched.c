/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
int tareas [8]={1,1,1,1,1,1,1,1};
int banderas [8]={1,1,1,1,1,1,1,1};
int current = 0;

void sched_inicializar() {
}


unsigned short sched_proximo_indice() {
	current +=1;
	while(tareas[current] == 0) current +=1;
	return current;
}

