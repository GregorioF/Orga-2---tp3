/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void screen_pintar_pantalla(){
	char c1 = 0;
	char c2 = 0x07;
	char* p = (char*)0xb8000;

	int i = 0;
	int k = 0;
	while( i < VIDEO_FILS){

		k = 0;
		while(k < VIDEO_COLS){
			*(p+k) = c1;
			*(p+k+1) = c2;

			k+=2;
		}

		p = p+80;
		i++;
	}

	

}
