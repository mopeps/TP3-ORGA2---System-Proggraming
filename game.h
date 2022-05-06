/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas al juego.
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "types.h"
#include "sched.h"

typedef enum e_task_type {
  Rick = 1,
  Morty = 2,
  Meeseeks = 3,
} task_type_t;

void game_init(void);

uint8_t aparecio_sobre_semilla(uint32_t pos_x,uint32_t pos_y);
uint32_t rick_score;
uint32_t morty_score;
void print_scores();
int abs(int number);
int cant_semillas;
void sumar_puntos(uint32_t);
bool game_over_excep;
bool except_from; //True la excepcion viene de rick y False de morty
uint32_t desplazar_meeseeks(int des_x, int des_y, uint16_t segmento_tarea);

void printEstadoTareas(void);
void avanzarSimbolo(uint16_t segmento_tarea);

bool estaSobreSemilla(uint16_t segmento_tarea);
bool estaSobreContrario(uint16_t segmento_tarea);
#endif //  __GAME_H__
