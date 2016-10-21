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

void mmu_inicializar_dir_tarea() {
	
	int* page_task_directory = (int*) nextPage();

	int page_t1_dir =  nextPage();
	page_task_directory[0]  = page_t1_dir | 0x7;

	int page_t2_dir = nextPage();
	page_task_directory[1] = page_t2_dir | 0x7; 

	int page_t3_dir = nextPage();
	page_task_directory[256] = page_t3_dir | 0x7;

	int* page_t1 = (int*) page_t1_dir;
	int* page_t2 = (int*) page_t2_dir;
	int* page_t3 = (int*) page_t3_dir;
	
	int i = 0;
	for (  i = 2 ; i < 1024; i++ ) page_directory[i] = 0; 
	for (  i = 0 ; i < 1024; i++ ) page_t1[i] = (i+1024*0) << 12 | 7;
	for (  i = 0 ; i < 768; i++ ) page_t2[i] = (i+1024*1) << 12 | 7; 
	
	page_t3[0] = 0x100007;
	page_t3[1] = 0x101007;
	page_t3[2] = 0x000007;
	
	unsigned char* codigo = (unsigned char*) 0x10000;
	unsigned char* destino = (unsigned char*) 0x100000;

	for ( i = 0; i < 12000; i++ ){
		destino[i] = codigo[i];
	}
		
}
 
