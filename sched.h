/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "tss.h"
#include "types.h"
#include "screen.h"
#include "defines.h"

void sched_init();
uint16_t sched_next_task();
void desalojar_tarea_actual(uint16_t segmento_tarea);
uint16_t taskList[256]; // las posiciones de este arreglo,comparten el indice de descriptores de la GDT de las TSS,y almacenan a que jugador les pertenece
uint16_t cantidadDeTareas;
uint16_t indice_tarea_rick;
uint16_t indice_tarea_morty;
uint16_t ultimo_indice_rick;
uint16_t ultimo_indice_morty;
uint8_t estado_tareas[25][2]; // 0-9 meeseeks jugador rick, 10-19 meeseeks jugador morty , 23 rick , 24, morty  | 1{segmento tarea, si es 0 significa que no hay una tarea en ese estado} 2{estado de la tarea }
uint8_t numero_rick;
uint8_t numero_morty;
uint16_t turno;

#endif //  __SCHED_H__
