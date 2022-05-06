/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de tipos comunes en el kernel.
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

typedef uint32_t vaddr_t; // direccion virtual.
typedef uint32_t paddr_t; // direccion fisica.

typedef struct str_page_dir_entry{
	uint8_t		present:1;
	uint8_t		read_write:1;
	uint8_t		user_supervisor:1;
	uint8_t		page_write_through:1;
	uint8_t		page_cache_disable:1;
	uint8_t		accessed:1;
	uint8_t		x:1;
	uint8_t		page_size:1;
	uint8_t		ignored:1;
	uint8_t		available:3;
	uint32_t	page_table_base:20;
}__attribute__((__packed__)) page_dir_entry;

typedef struct str_page_table_entry{
	uint8_t		present:1;
	uint8_t		read_write:1;
	uint8_t		user_supervisor:1;
	uint8_t		page_write_through:1;
	uint8_t		page_cache_disable:1;
	uint8_t		accessed:1;
	uint8_t		dirty:1;
	uint8_t		x:1;
	uint8_t		global:1;
	uint8_t		available:3;
	uint32_t	physical_address_base:20;
}__attribute__((__packed__)) page_table_entry;


uint32_t pos_semillas[20][2];
uint32_t pos_meeseks[40][5]; // 0: pos_x | 1: pos_y | 2: jugador | 3: limite_movimientos | 4: uso_portalgun?
uint32_t dir_comienzo_meeseeks[40][5]; // 0: virt_addr | 1: phy_addr | 2: codigo_tarea | 3: cr3_actual | 4: pila nivel 0
bool meeseeks_move_clock[40];

#endif //  __TYPES_H__
