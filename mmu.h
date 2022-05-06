/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "types.h"


void mmu_init(void);

paddr_t mmu_next_free_kernel_page(void);

vaddr_t mmu_next_free_virtual_task_page_morty(void);
vaddr_t mmu_next_free_virtual_task_page_rick(void);

void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs);

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt);

paddr_t mmu_init_kernel_dir(void);

paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start,vaddr_t vir_target, size_t pages);
	


paddr_t mmu_next_free_task_page(void);
#endif //  __MMU_H__
