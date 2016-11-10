/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"

unsigned int game_fondear(unsigned int ancla_dir_fisica, unsigned int cr3);
unsigned int game_canonear(unsigned int dir_usuario, unsigned int dir_relativa);
unsigned int game_navegar(unsigned int cr3, unsigned int dir_primera_pag, unsigned int dir_segunda_pag);

#endif  /* !__GAME_H__ */
