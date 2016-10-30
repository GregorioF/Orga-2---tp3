/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"

static const char* a []= {
"ERROR 0: DIVIDE ERROR", "ERROR 1: RESERVED", "ERROR 2: NMI Interrupt", "ERROR 3: Breakpoint", "ERROR 4: Overflow", "ERROR 5: BOUND Range Exceeded", "ERROR 6: Invalid Opcode (Undefined Opcode)", "ERROR 7: Device Not Available (No Match Coprocessor)", "ERROR 8: Double Fault", "ERROR 9: Coprocessor Segment Overrun (reserved)", "ERROR 10: Invalid TSS", "ERROR 11: Segment Not Present", "ERROR 12: Stack-Segment Fault", "ERROR 13: General Protection", "ERROR 14: Page Fault", "ERROR 15: (Intel reserved. Do not use.)", "ERROR 16: x87 FPU Floating-Point Error (Math Fault)", "ERROR 17: Alignment Check", "ERROR 18: Machine Check", "ERROR 19: SIMD Floating-Point Exception"
};


char tabla_traduccion (int eax) {
	char uno = '1';
	if (eax >= 0x2  && eax < 0xb ){
		return uno + eax-2;
	}
	if (eax == 0xb) return '0';
	return 0;
}

void print_numerito(int eax){
	if (eax == 0x32) imprimir_banderitas();
	else{ 

		char numerito = tabla_traduccion(eax);
		ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
		ca temp = {.c = numerito, .a = C_BG_RED  | C_FG_WHITE};
		p[0][79] = temp;
		}
}

void imprimir_texto(char* palabra, int n, int currFila, int currCol ){
	
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	
	int i = currCol;
	for( i = currCol; i < n+currCol ; i ++ ){
		ca temp = {.c = palabra[i-currCol], .a = C_BG_BLACK  | C_FG_WHITE};
		p[currFila][i] = temp;
	}
		
}

void imprimir_nombre_del_grupo(){
	char* c = "El Arquitecto";
	imprimir_texto( c, 13, 0 ,  1);
		
}

void imprimir_banderitas(){
	
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	unsigned int i;


	//IMPRIMIMOS FONDO
	unsigned int j;
	for (i = 0; i < VIDEO_FILS; i++){
			for (j = 0; j < VIDEO_COLS; j++){
					ca temp = {.c = 0, .a = C_BG_LIGHT_GREY  | C_FG_BLACK};
					p[i][j] = temp;
			}
	}


	//IMPRIMIMOS EN LA PRIMER LINEA EL NOMBRE DEL GRUPO
	char* nombreGrupo = "El Arquitecto";
	int r = 0;
	for (i = 0; i < VIDEO_COLS; i++) {
		if( r < 13){ 
			ca temp = {.c = nombreGrupo[r], .a = C_BG_BLACK | C_FG_WHITE };
			r++;
			p[0][i] = temp;
		}else{ 
			ca temp = {.c = ' ', .a = C_BG_BLACK | C_FG_WHITE };
			p[0][i] = temp;
		}
	}
	

	//ARMAMOS SECCION DE ULTIMO ERROR 
	for (i = 2; i < 15; i++){
			for(j = 50; j < 79; j++){
				ca temp = {.c = 0, .a = C_BG_BLACK | C_FG_WHITE };
				p[i][j] = temp;
			}
	}
	//PONEMOS FRAJNA AZUL SOBRE LA SECCION DE ERROR
	for (i = 50; i < 79; i++){
			ca temp = {.c = 0, .a = C_BG_CYAN | C_FG_BLACK};
			p[1][i] = temp;
	}
	

	//PONEMOS FRANJA NARANJA SOBRE LA SECCION  DE ESTADOS
	for (i = 2; i < 79; i++){
			ca temp = {.c = 0, .a = C_BG_BROWN| C_FG_WHITE};
			p[16][i] = temp;
	}
	
	//ARMAMOS LA SECCION DE ESTADOS
	for (i = 2; i < 79; i++){
		for (j = 17; j < 24; j++){
			ca temp = {.c = 0, .a = C_BG_CYAN | C_FG_BLACK };
				p[j][i] = temp;
		}
	}
	//PONEMOS LA ULTIMA LINEA EN NEGRO 
	for (i = 0; i < VIDEO_COLS; i++){
			ca temp = {.c = 0, .a = C_BG_BLACK | C_FG_WHITE };
			p[24][i] = temp;
	}
	//BORDE IZQUIERDO DE SECCION ESTADOS
	for (i = 16; i < 24; i++){
		ca temp = {.c = (i-15)+'0', .a = C_BG_LIGHT_GREY | C_FG_BLACK };
		p[i][1] = temp;
		ca temp2 = {.c = 0, .a = C_BG_BLACK | C_FG_WHITE  };
		p[i][0]= temp2;
		p[i][79]= temp2;
	}

	for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+3][i+2] = temp;
		}
	}
	for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+3][i+14] = temp;
		}
	}for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+3][i+26] = temp;
		}
	}for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+3][i+38] = temp;
		}
	}for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+10][i+2] = temp;
		}
	}for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+10][i+14] = temp;
		}
	}for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+10][i+26] = temp;
		}
	}for (i = 0; i < 10 ; i ++){
		for (j = 0; j < 5; j++){
			ca temp = {.c = 0, .a = C_BG_MAGENTA | C_FG_BLACK };
			p[j+10][i+38] = temp;
		}
	}

}
	

void printear(unsigned int x){
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	unsigned int i;
	for (i = 0; a[x][i] != 0; i++) {
		ca temp = {.c = a[x][i], .a = C_BG_BLACK | C_FG_WHITE };
		p[0][i+1] = temp;
	}
    
}


void limpiar_pantalla (){
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	unsigned int i;
	unsigned int k;
	
	for (i = 0; i < VIDEO_FILS; i++) {
		for( k = 0 ; k < VIDEO_COLS ; k ++){
				ca temp = {.c = 0, .a = C_BG_BLACK | C_FG_WHITE };
				p[i][k] = temp;
			}
		
	}
	
}

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}


void screen_pintar_pantalla(){
	char c1 = 0;
	char c3 = 42;
	char c4 = 0xff;
	char c2 = 0x07;
	char* p = (char*)0xb8000;

	int i = 1;
	int k = 0;
	
	while(k < VIDEO_COLS){
			*(p+k) = c3;
			*(p+k+1) = c4;

			k+=2;
		}
		p = p+80;
	
	while( i < VIDEO_FILS-1){

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


