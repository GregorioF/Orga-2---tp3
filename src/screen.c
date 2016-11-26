/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"
#define BANDERA_BUFFER  0x40001000

static const char* a []= {
"DIVIDE ERROR", "RESERVED", "NMI Interrupt", "Breakpoint", "Overflow", "BOUND Range Exceeded", "Invalid Opcode", 
"Device Not Available", "Double Fault", "Coprocessor Segment Overrun", "Invalid TSS", "Segment Not Present", 
"Stack-Segment Fault", "General Protection", "Page Fault", "RESERVED", "Floating-Point Error", "Alignment Check", "Machine Check", "SIMD Floating-Point Exception"
};

static const char* registros[] = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP", "EIP", "CR0", "CR2", "CR3", "CR4", "CS", "DS", "ES", "FS", "GS", "SS", "EFLAGS" };
int mapa = 0;
ca flags[8][5][10];
int posicionTareas [8][3];
char paginasTareas [8][4][8];
int ultimoMisil = -1;
char fondo[80];
int ultimoError = -1;
int ultimoNavio = -1;
static const char navio[6] = {"NAVIO "}; 
static const char bandera[28] = {"ERROR BANDERA               "};
char debug[20][8];

char clocksTareas [] = {'|', '|', '|', '|', '|', '|', '|', '|'};
char clocksBandeas[] = {'|', '|', '|', '|', '|', '|', '|', '|'};

void debugger(unsigned int cr0, unsigned int cr2, unsigned int cr3, unsigned int cr4, unsigned int fs, unsigned int gs, 
unsigned int es, unsigned int ds, unsigned int edi, unsigned int esi, unsigned int ebp, unsigned int esp2, unsigned int ebx, 
unsigned int edx, unsigned int ecx, unsigned int eax, unsigned int ss, unsigned int esp, unsigned int eflags, unsigned int cs, unsigned int eip){
		guardar_hex(eax, 0);
		guardar_hex(ebx, 1);
		guardar_hex(ecx, 2);
		guardar_hex(edx, 3);
		guardar_hex(esi, 4);
		guardar_hex(edi, 5);
		guardar_hex(ebp, 6);
		guardar_hex(esp, 7);
		guardar_hex(eip, 8);
		guardar_hex(cr0, 9);
		guardar_hex(cr2, 10);
		guardar_hex(cr3, 11);
		guardar_hex(cr4, 12);
		guardar_hex(cs, 13);
		guardar_hex(ds, 14);
		guardar_hex(es, 15);
		guardar_hex(fs, 16);
		guardar_hex(gs, 17);
		guardar_hex(ss, 18);
		guardar_hex(eflags, 19);
}


void printearError(short n,unsigned int error){
	
	paginasTareas[n][3][0] = error+48;
	ultimoError = error;
	ultimoNavio = n;
	imprimir_banderitas();
	mostrar_mapa();
}

void inicializar_fondo(){
	int j = 0;
	for (j = 0; j < 80; j++){
		fondo[j] = 0;
	}
	fondo[3] = 'P';
	fondo[4] = '1';
	fondo[5] = ':';
	for (j=6; j  < 16; j++){
		fondo[j] = '0';
	}
	fondo[7] = 'x';
		
	fondo[17] = 'P';
	fondo[18] = '2';
	fondo[19] = ':';
	for (j=20; j  < 30; j++){
		fondo[j] = '0';
	}
	fondo[21] = 'x';
	fondo[31] = 'P';
	fondo[32] = '3';
	fondo[33] = ':';
	for (j=34; j  < 44; j++){
		fondo[j] = '0';
	}
	fondo[35] = 'x';
	
}

void inicializar_flags(){
	int i = 0;
	int j = 0;
	int k = 0;
	for (i = 0; i < 8; i++){
		for (j = 0; j < 5; j++){
			for (k = 0;k < 10; k++){
				ca temp = {.c = 0, .a = C_BG_RED | C_FG_BLACK };
				flags[i][j][k] = temp;
			}
		}
	}
}
void borrarDelMapa(unsigned int n){
	posicionTareas[n][0] = -1;
	posicionTareas[n][1] = -1;
	posicionTareas[n][2] = -1;
}

void inicializar_mapa(){
	int i = 0;
	for(i = 0; i < 8; i++){
		posicionTareas[i][0] = (1048576 + (8192)*i)/4096;
		posicionTareas[i][1] = (1052672 + (8192)*i)/4096;
		posicionTareas[i][2] = 0;	
		int j = 0;
		int k = 7;
		for (j = 0; j < 8; j++){
			paginasTareas[i][0][j] = devolver_hex(0x100000 + (0x2000)*i,k);
			paginasTareas[i][1][j] = devolver_hex(0x101000 + (0x2000)*i,k);
			paginasTareas[i][2][j] = '0';
			k-=1;
		}
		
		paginasTareas[i][3][0] = 0;
	}
	ponerUltimaBarra();
}

char tabla_traduccion (int eax) {
	char uno = '1';
	if (eax >= 0x2  && eax < 0xb ){
		return uno + eax-2;
	}
	if (eax == 0xb) return '0';
	return 0;
}

void mostrar_mapa(){
	
	if(mapa){
		int i = 0;
		
		ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
		int fil = 0;
		int col = 0;
		for ( fil = 0; fil < 25; fil++ ){
			for ( col = 0; col < 80 ; col++){
				ca mar = {.c = 0, .a = C_BG_BLUE  | C_FG_WHITE};
				p[fil][col] = mar;
				if ((fil*VIDEO_COLS) + col < 256 ){
						ca tierra = {.c = 0, .a = C_BG_GREEN  | C_FG_WHITE};
						p[fil][col] = tierra;
				}
			}
		}
		ca reloj_idle = {.c = 0, .a = C_BG_LIGHT_GREY | C_FG_BLACK};
		p[24][0] = reloj_idle;
		for (col = 1; col < 80 ; col++){
			ca borde = {.c = 0, .a = C_BG_BLACK | C_FG_WHITE};
			p[24][col] = borde;
		}
		
		for (i=0; i<8; i++){				
			int j = 0;
			for (j=0; j<3; j++){
				
				int n = posicionTareas[i][j];
				if (n != -1){
								
					col = n;
					fil = 0;
					while ( col >= VIDEO_COLS ){
							col = col - VIDEO_COLS;
							fil = fil + 1;
					}
					
					if (p[fil][col].c != 0){
						ca tareas = {.c = 'x' , .a = C_BG_RED  | C_FG_WHITE};
						p[fil][col] = tareas;
					}
					else{
						ca tareas = {.c = i+49 , .a = C_BG_BROWN  | C_FG_WHITE};
						p[fil][col] = tareas;
					}
				}
			}
		}
		
		if ( ultimoMisil != -1 ){
			fil = 0;
			col = ultimoMisil;
			while ( col >= VIDEO_COLS ){
						col = col - VIDEO_COLS;
						fil = fil + 1;
			}
			
			ca misil = {.c = 0 , .a = C_BG_CYAN  | C_FG_WHITE};
			p[fil][col] = misil;
		}
	}
		 
}

void print_numerito(int eax){
	if (eax == 0x12){
		mapa = 0;
		imprimir_banderitas();
	}
	else{
		if(eax == 0x32){
			mapa = 1;
			mostrar_mapa();
			
		}
		else{ 
			char numerito = tabla_traduccion(eax);
			ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
			ca temp = {.c = numerito, .a = C_BG_RED  | C_FG_WHITE};
			p[0][79] = temp;
		}
	}
}



void actualizar_mapa(unsigned int n, unsigned int m, unsigned int movimiento, unsigned int current){
		
		if (movimiento == 0){
			//fondear:
			if(posicionTareas[current][0] != -1){
				posicionTareas[current][3] = (n/4096);
				int i = 0;
				int k = 7;
				for (i=0; i < 8; i++){
					paginasTareas[current][2][i] = devolver_hex(n,k);
					k-=1;
				}
			}
		}
		if (movimiento == 1){
			if(posicionTareas[current][0] != -1){
				//navegar:
				posicionTareas[current][0] = (n/4096);
				posicionTareas[current][1] = (m/4096);	
				int i = 0;
				int k = 7;
				for (i=0; i < 8; i++){
					paginasTareas[current][0][i] = devolver_hex(n,k);
					paginasTareas[current][1][i] = devolver_hex(m,k);
					k-=1;
				}
			}
		}
		if (movimiento == 2){
			//lanzar misil:
			ultimoMisil = (n/4096);
		}
		
		mostrar_mapa();
		ponerUltimaBarra();
}


void imprimir_banderitas(){
	
	if(mapa==0){
	
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
				for (j = 50; j < 79; j++){
					ca temp = {.c = 0, .a = C_BG_BLACK | C_FG_WHITE };
					p[i][j] = temp;
					}
				for (j = 51; registros[i-2][j-51]!= 0; j++){
					p[i][j].c = registros[i-2][j-51];
				}
				for(j = 55; j < 63; j++){
					ca temp = {.c = debug[i-2][j-55], .a = C_BG_BLACK | C_FG_WHITE };
					p[i][j] = temp;
				}
				
		}
		for (i = 2; i  <  8; i++){
			for (j = 67; registros[i+11][j-67]!= 0; j++){
					p[i][j].c = registros[i+11][j-67];
				}
			for (j = 70; j < 78; j++){
				ca temp = {.c = debug[i+11][j-70], .a = C_BG_BLACK | C_FG_WHITE };
				p[i][j] = temp;

			}
		}
		
		p[9][67].c = 'E';
		p[9][68].c = 'F';
		p[9][69].c = 'L';
		p[9][70].c = 'A';
		p[9][71].c = 'G';
		p[9][72].c = 'S';
		for (i = 70; i < 78; i++){
			ca temp = {.c = debug[19][i-70], .a = C_BG_BLACK | C_FG_WHITE };
			p[10][i] = temp;
		}
		//PONEMOS FRANJA AZUL SOBRE LA SECCION DE ERROR
		
		int aux = 1;
		for (i = 50; i < 79; i++){
				ca temp = {.c = 0, .a = C_BG_CYAN | C_FG_BLACK};
				if (ultimoError != -1){
					
					if ( aux && ultimoError != 20){
						aux = a[ultimoError][i-50] != 0;
					}
					if (aux && ultimoError != 20){
						temp.c = a[ultimoError][i-50];
					}
					if (ultimoError == 20){
						temp.c = bandera[i-50];
					}
					
					if ( 71 < i &&  i < 77){
						temp.c = navio[i-72];
					}
					if ( i == 78 ){
						temp.c = ultimoNavio + 49;
					}
				}
				p[1][i] = temp;
		}
		
		//ARMAMOS LA SECCION DE ESTADOS
		for (i = 2; i < 79; i++){
			for (j = 16; j < 24; j++){
				ca temp = {.c = fondo[i], .a = C_BG_CYAN | C_FG_BLACK };
				if (paginasTareas[j-16][3][0] != 0){
					temp.a = C_BG_BROWN | C_FG_BLACK;
				}
				p[j][i] = temp;
			}
		}
		for (j = 16;j < 24; j++){
			for (i = 8; i < 16; i++){
				p[j][i].c = paginasTareas[j-16][0][i-8];
			}
			for (i = 22; i < 30; i++){
				p[j][i].c = paginasTareas[j-16][1][i-22];
			}
			for (i = 36; i < 44; i++){
				p[j][i].c = paginasTareas[j-16][2][i-36];
			}
			char num = (paginasTareas[j-16][3][0]);
			if (num != 0 && (num-48) < 20){
				for (i = 55; a[num-48][i-55] != 0 ; i++){
					p[j][i].c = a[num-48][i-55];
				}
			}
			else{
				if((num-48) == 20){
					for (i = 55; bandera[i-55] != 0 ; i++){
						p[j][i].c = bandera[i-55];
					}	
				}
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
				ca temp = flags[0][j][i];
				p[j+3][i+2] = temp;
			}
		}
		
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[1][j][i];
				p[j+3][i+14] = temp;
			}
		}
		
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[2][j][i];
				p[j+3][i+26] = temp;
			}
		}
		
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[3][j][i];
				p[j+3][i+38] = temp;
			}
		}
		
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[4][j][i];
				p[j+10][i+2] = temp;
			}
		}
		
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[5][j][i];
				p[j+10][i+14] = temp;
			}
		}
		
		
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[6][j][i];
				p[j+10][i+26] = temp;
			}
		}
		for (i = 0; i < 10 ; i ++){
			for (j = 0; j < 5; j++){
				ca temp = flags[7][j][i];
				p[j+10][i+38] = temp;
			}
		}

		ponerUltimaBarra();

	}
}

void ponerUltimaBarra(){
	int j = 0; 
	int i = 0;
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	for (i = 4; i < 28; i+=3){
		char asd [] = {'1', '2', '3', '4', '5', '6', '7', '8'};
		ca temp = {.c = asd[j], .a = C_BG_LIGHT_GREY | C_FG_BLACK };
		
		p[24][i]= temp;
		temp.c = clocksTareas[j];
		p[24][i+1] = temp;

		temp.c = asd[j];
		temp.a = C_BG_MAGENTA | C_FG_WHITE;
		p[24][i+28] = temp;

		temp.c = clocksBandeas[j];
		temp.a = C_BG_MAGENTA | C_FG_WHITE;
		p[24][i+29] = temp;
		j++;	
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

char devolver_hex(unsigned int numero,unsigned int n) {
    char hexa[] = {0,0,0,0,0,0,0,0};
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
	return hexa[n];
}

void guardar_hex(unsigned int numero, unsigned int x) {
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
    for(i = 0; i < 8; i++) {
		debug[x][i] = hexa[i];
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

void imprimir_nombre_del_grupo(){
	char* c = "El Arquitecto";
	imprimir_texto( c, 13, 0 ,  1);		
}


void screen_pintar_pantalla(){
	/*
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
	*/
	
}
void actualizar_bandera(unsigned short n){
	
	ca(*buffer)[10] = (ca(*)[10]) (BANDERA_BUFFER);
 
    unsigned int fil;
    unsigned int col;
	unsigned int i;
	
    for (fil = 0; fil < 5; fil++) {
        for (col = 0; col < 10; col++) {
            flags[n][fil][col] = buffer[fil][col];
            i = i+1;
        }
    }
}

void moverClockBandera(int bandera ){	
	char c = clocksBandeas [bandera];
	clocksBandeas [bandera] = siguienteReloj(c);

}

void moverClockTarea(int tarea ){	
	char c = clocksTareas [tarea];
	clocksTareas [tarea] = siguienteReloj(c);

}


char siguienteReloj(char c){
	if( c == '|') return '/';
	else if (c == '/') return '-';
	else if (c == '-') return 92;
	else return '|';
}
