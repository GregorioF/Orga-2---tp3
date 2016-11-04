/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "gdt.h"
#include "mmu.h" 

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

int dir_tareas []  = {0x31000, 0x32000, 0x33000, 0x34000, 0x35000, 0x36000, 0x37000, 0x38000};
void actualizar_gdt(){
	int i = 0;
	int j = 0;
	
	
    gdt[TAREA_INICIAL] = (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)    (unsigned long int)&tarea_inicial & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     ((unsigned long int)&tarea_inicial & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     ((unsigned long int)&tarea_inicial & 0xff000000) >> 24,           /* base[31:24]  */
    };
    gdt[TAREA_IDLE] = (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)    (unsigned long int)&tarea_idle & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     ((unsigned long int)&tarea_idle & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     ((unsigned long int)&tarea_idle & 0xff000000) >> 24,           /* base[31:24]  */
    };
	
	
	for(i  = 25, j = 0 ; i < 33 && j  < 8 ; i++, j++){
		
			gdt[i] = (gdt_entry){
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)    ((unsigned long int)&tss_navios[j]) & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     ((unsigned long int)(&tss_navios[j]) & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     (((unsigned long int)&tss_navios[j]) & 0xff000000) >> 24,           /* base[31:24]  */
		};    
	}
	
	for(i = 33, j = 0; i < 41 && j < 8 ; i ++, j++ ){
		gdt[i]= (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)    (unsigned long int)&tss_banderas[j] & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     ((unsigned long int)&tss_banderas[j] & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     ((unsigned long int)&tss_banderas[j] & 0xff000000) >> 24,           /* base[31:24]  */
		};
	
	}
}



void tss_inicializar() {
	actualizar_gdt();
	
	tarea_idle.eip = 0x20000;
	tarea_idle.esp0 = 0x2A000;
	tarea_idle.esp = 0x2A000;
	tarea_idle.ebp = 0x2A000;
	tarea_idle.eflags = 0x202;
	tarea_idle.cs = GDT_COD_L0 << 3;
	tarea_idle.ds = GDT_DAT_L0 << 3;
	tarea_idle.ss = GDT_DAT_L0 << 3;
	tarea_idle.cr3 = PAGE_DIRECTORY >> 12;
	tarea_idle.cr3 = tarea_idle.cr3 << 12;

   
    int i = 0;
    for (i = 0; i < 8 ; i ++){
        tss_navios[i].eip = 0x40000000;
       // tss_navios[i].esp3 = 0x40001C00;  BRIANGATUBELA
        tss_navios[i].esp = 0x40001C00;
        tss_navios[i].ebp = 0x40001C00;
        tss_navios[i].eflags = 0x202;  
        tss_navios[i].esp0 = nextPage();
        tss_navios[i].cs = GDT_COD_L3 << 3;
        tss_navios[i].ds = GDT_DAT_L3 << 3;
        tss_navios[i].ss = GDT_DAT_L3 << 3;
        tss_navios[i].cr3 = dir_tareas[i];
        mmu_inicializar_dir_tarea(i, dir_tareas[i]) ;
		
		tss_banderas[i].eip = *(char*)0x40001FFF;
        tss_banderas[i].esp = 0x40001C00;
        tss_banderas[i].ebp = 0x40001C00;
        tss_banderas[i].eflags = 0x202;  
        tss_banderas[i].esp0 = nextPage();
        tss_banderas[i].cs = GDT_COD_L3 << 3;
        tss_banderas[i].ds = GDT_DAT_L3 << 3;
        tss_banderas[i].ss = GDT_DAT_L3 << 3;
        tss_banderas[i].cr3 = dir_tareas[i];
    
    }
    
    
	
	

}

