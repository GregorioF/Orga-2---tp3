/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__


/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80
#define VIDEO_SCREEN 0xB8000

#include "colors.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

char devolver_hex(unsigned int numero, unsigned int n);
void guardar_hex(unsigned int numero, unsigned int x);
void mostrar_mapa();
void printearError(short n,unsigned int error);
void printear(unsigned int);
void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void imprimir_banderitas();
void imprimir_texto(char* c,  int a , int b ,int d);
void imprimir_nombre_del_grupo();

void screen_pintar_pantalla();
void actualizar_mapa(unsigned int n, unsigned int m, unsigned int movimiento, unsigned int current);
void ponerUltimaBarra();
void moverClockBandera(int bandera );
char siguienteReloj(char c);
void moverClockTarea(int tarea);
void modificarClock(short n );

#endif  /* !__SCREEN_H__ */
