/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__
/* MMU */
/* -------------------------------------------------------------------------- */

#define MMU_P (1 << 0)
#define MMU_W (1 << 1)
#define MMU_U (1 << 2)

#define PAGE_SIZE 4096

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40

// X Columnas
#define SIZE_M 80

// nro Semillas
#define TOTAL_SEMILLAS 20
/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC 0
#define GDT_COUNT         35


#define GDT_IDX_CODIGO_0 10 
#define GDT_IDX_DATO_0 11 
#define GDT_IDX_CODIGO_3 12 
#define GDT_IDX_DATO_3 13 
#define SEGMENTO_IDX_PANTALLA 14 
#define GDT_IDX_TSS_INIT 15
#define GDT_IDX_TSS_IDLE 16
#define GDT_IDX_TSS_RICK 17
#define GDT_IDX_TSS_MORTY 18
//#define GDT_IDX_TSS_MEESEEKS_R1 19
//#define GDT_IDX_TSS_MEESEEKS_R2 20
//#define GDT_IDX_TSS_MEESEEKS_M1 21
//#define GDT_IDX_TSS_MEESEEKS_M2 22
/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_CODIGO_0 (GDT_IDX_CODIGO_0 << 3)
#define GDT_OFF_DATO_0 (GDT_IDX_DATO_0 << 3)
#define GDT_OFF_CODIGO_3 (GDT_IDX_CODIGO_3 << 3) 
#define GDT_OFF_DATO_3 (GDT_IDX_DATO_3 << 3) 

#define SEGMENTO_OFF_PANTALLA (SEGMENTO_IDX_PANTALLA << 3)
#define GDT_OFF_TSS_INIT  (GDT_IDX_TSS_INIT << 3)
#define GDT_OFF_TSS_IDLE  (GDT_IDX_TSS_INIT << 3)
#define GDT_OFF_TSS_RICK  (GDT_IDX_TSS_RICK << 3)
#define GDT_OFF_TSS_MORTY  (GDT_IDX_TSS_MORTY << 3)
#define GDT_OFF_TSS_MEESEEKS_R1 (GDT_IDX_TSS_MEESEEKS_R1 << 3)
#define GDT_OFF_TSS_MEESEEKS_R2 (GDT_IDX_TSS_MEESEEKS_R2 << 3)
#define GDT_OFF_TSS_MEESEEKS_M1 (GDT_IDX_TSS_MEESEEKS_M1 << 3)
#define GDT_OFF_TSS_MEESEEKS_M2 (GDT_IDX_TSS_MEESEEKS_M2 << 3)

#define GDT_RPL_CODIGO_0 0x0    
#define GDT_RPL_DATO_0 0x0
#define GDT_RPL_CODIGO_3 0x3    
#define GDT_RPL_DATO_3 0x3

/* INTERRUPCIONES */
#define INTERRUPT_0_ATTR 0x8E00
#define INTERRUPT_3_ATTR 0xEE00

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */

// direccion virtual del codigo
#define TASK_CODE_VIRTUAL 0x01D00000
#define TASK_PAGES        4
#define TASK_CODE 0x8000000
#define TASK_TASKS_CODE 0x10000

#define TASK_IDLE_CODE 0x00018000 
/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<X>_PHY_START.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR     (0x00025000)
#define KERNEL_PAGE_TABLE_0 (0x00026000)
#define KERNEL_STACK        (0x00025000)

/*TASKS*/

#define DEFAULT_FLAGS_VALUES 0x00000202


#endif //  __DEFINES_H__
