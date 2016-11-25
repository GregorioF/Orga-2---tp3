/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "screen.h"
#include "i386.h"


unsigned int game_fondear(unsigned int current,unsigned int ancla_dir_fisica, unsigned int cr3) {
	mmu_mapear_pagina(0x40002000,ancla_dir_fisica,cr3);	
	actualizar_mapa(ancla_dir_fisica,0,0,current);
    return TRUE;
}

unsigned int game_canonear(unsigned int current, unsigned int dir_misil_fisica, unsigned int dir_buffer_absoluta) {
	unsigned char* misil = (unsigned char*) dir_misil_fisica;  
	unsigned char* destino = (unsigned char*) dir_buffer_absoluta;
	unsigned int i = 0;
	for ( i = 0; i < 97; i++ ){
		destino[i] = misil[i];
	}
	
	actualizar_mapa(dir_misil_fisica,0,2,current);
	
    return TRUE;
}

unsigned int game_navegar( unsigned int current, unsigned int cr3, unsigned int dir_primera_pag_fisica, unsigned int dir_segunda_pag_fisica) {
	
	//breakpoint();
	unsigned char* codigoPag1 = (unsigned char*) 0x40000000;
	unsigned char* destinoPag1 = (unsigned char*) dir_primera_pag_fisica;
	unsigned char* codigoPag2 = (unsigned char*) 0x40001000;
	unsigned char* destinoPag2 = (unsigned char*) dir_segunda_pag_fisica;
	unsigned int i = 0;
	for ( i = 0; i < 4096; i++){
		destinoPag1[i] = codigoPag1[i];
		destinoPag2[i] = codigoPag2[i];
	}
	
	mmu_mapear_pagina(0x40001000, cr3, dir_segunda_pag_fisica);
	mmu_mapear_pagina(0x40000000, cr3,dir_primera_pag_fisica);
	
	actualizar_mapa(dir_primera_pag_fisica,dir_segunda_pag_fisica,1,current);
	//breakpoint();
	
    return TRUE;
}



