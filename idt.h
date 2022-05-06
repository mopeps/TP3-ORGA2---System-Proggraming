/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas de atencion de interrupciones
*/

#ifndef __IDT_H__
#define __IDT_H__

#include "types.h"
#include "i386.h"

/* Struct de descriptor de IDT */
typedef struct str_idt_descriptor {
  uint16_t idt_length;
  uint32_t idt_addr;
} __attribute__((__packed__)) idt_descriptor_t;

/* Struct de una entrada de la IDT */
typedef struct str_idt_entry_fld {
  uint16_t offset_15_0;
  uint16_t segsel;
  uint16_t attr;
  uint16_t offset_31_16;
} __attribute__((__packed__, aligned(8))) idt_entry_t;

extern idt_entry_t idt[];
extern idt_descriptor_t IDT_DESC;

uint8_t debug_flag = 0;
void set_debug_flag(uint8_t val);
uint8_t read_debug_flag();

uint8_t debug_interface_flag = 0;
void set_debug_interface_flag(uint8_t val);
uint8_t read_debug_interface_flag();

void print_stack_err_trace(uint32_t ESP,uint32_t ebp,uint32_t segmento_tarea);
void idt_init(void);

#endif //  __IDT_H__
