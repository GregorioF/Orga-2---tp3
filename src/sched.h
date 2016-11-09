/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__


void sched_inicializar();
short sched_proximo_indice();
void inhabilitar_tarea(unsigned short n);
unsigned short sched_indice_actual();


#endif	/* !__SCHED_H__ */
