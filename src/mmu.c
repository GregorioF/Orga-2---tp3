/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "defines.h"
#include "i386.h"



int* page_directory = (int*)PAGE_DIRECTORY;
int* page_table_1 = (int*)PAGE_TABLE_1;
int* page_table_2 = (int*)PAGE_TABLE_2;

int PPL = 0x31000;

int nextPage(){
		PPL += 0x1000;
		return PPL;
}


void mmu_inicializar() {
	page_directory[0] = PAGE_TABLE_1 | 0X3;
	page_directory[1] = PAGE_TABLE_2 | 0X3;
	int i = 0;
	for (  i = 2 ; i < 1024; i++ ) page_directory[i] = 0; 
	for (  i = 896 ; i < 1024; i++ ) page_table_2[i] = 0; 
	for (  i = 0 ; i < 1024; i++ ) page_table_1[i] = (i+1024*0) << 12 | 3;
	for (  i = 0 ; i < 896; i++ ) page_table_2[i] = (i+1024*1) << 12 | 3; 
	
}

void mmu_inicializar_dir_tarea() {
	
	int* page_task_directory = (int*) 0x31000;

	int page_t1_dir =  nextPage();
	int page_t2_dir = nextPage();
	int page_t3_dir = nextPage();

	int* page_t1 = (int*) page_t1_dir;
	int* page_t2 = (int*) page_t2_dir;
	int* page_t3 = (int*) page_t3_dir;
	
	int i = 0;
	for (  i = 0 ; i < 1024; i++ ) page_task_directory[i] = 0; 

	page_task_directory[0]  = page_t1_dir | 0x7;
	page_task_directory[1] = page_t2_dir | 0x7; 
	page_task_directory[256] = page_t3_dir | 0x7;

	for (  i = 0 ; i < 1024; i++ ) page_t1[i] = (i+1024*0) << 12 | 7;
	for (  i = 0 ; i < 896; i++ ) page_t2[i] = (i+1024*1) << 12 | 7; 
	for (  i = 896 ; i < 1024; i++ ) page_t2[i] = 0; 
	
	for (  i = 0 ; i < 1024; i++ ) page_t3[i] = 0; 
	page_t3[0] = 0x100007;
	page_t3[1] = 0x101007;
	page_t3[2] = 0x000007;
	for (  i = 3 ; i < 1024; i++ ) page_t3[i] = 0; 
	
	unsigned char* codigo = (unsigned char*) 0x10000;
	unsigned char* destino = (unsigned char*) 0x100000;

	for ( i = 0; i < 12000; i++ ){
		destino[i] = codigo[i];
	}
		
}

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica){
	int directorio = cr3 >> 12;
	directorio = directorio << 12;
	int* pd = (int*) directorio;
	int pd_index = virtual >> 22;
	int pde = pd[pd_index];
	int user = ((pde  >> 2) % 2);

	int pagina = pde >> 12;
	pagina = pagina << 12;
	int* pt = (int*) pagina;
	int pte_index = virtual << 10;
	pte_index = pte_index  << 22;
	fisica = fisica >> 12;
	fisica = fisica << 12;
	if (user){
		pt[pte_index] = fisica | 7;
	}
	else{
		pt[pte_index] = fisica | 3;
	}

	tlbflush();
}

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	int directorio = cr3 >> 12;
	directorio = directorio << 12;
	int* pd = (int*) directorio;
	int pd_index = virtual >> 22;
	int pde = pd[pd_index];
	pde = pde >> 1;
	pde = pde << 1; // ahora la tabla de pagina no esta presente

	tlbflush();
} 
