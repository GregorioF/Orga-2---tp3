/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "defines.h"



int* page_directory = (int*)PAGE_DIRECTORY;
int* page_table_1 = (int*)PAGE_TABLE_1;
int* page_table_2 = (int*)PAGE_TABLE_2;

int PPL = 0x30000;

int nextPage(){
		PPL += 0x1000;
		return PPL;
}


void mmu_inicializar() {
	page_directory[0] = PAGE_TABLE_1 | 0X3;
	page_directory[1] = PAGE_TABLE_2 | 0X3;
	int i = 0;
	for (  i = 2 ; i < 1024; i++ ) page_directory[i] = 0; 
	for (  i = 0 ; i < 1024; i++ ) page_table_1[i] = (i+1024*0) << 12 | 3;
	for (  i = 0 ; i < 768; i++ ) page_table_2[i] = (i+1024*1) << 12 | 3; 
	
}
 
