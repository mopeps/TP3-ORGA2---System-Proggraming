/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"


uint32_t free_kernel_page;
uint32_t free_task_page;
uint32_t free_virtual_task_page_morty;
uint32_t free_virtual_task_page_rick;

extern void copy_mem(paddr_t from, size_t pages);

void mmu_init(void) {
	free_kernel_page = 0x100000;
	free_task_page = 0x400000;
	free_virtual_task_page_morty = TASK_CODE;
	free_virtual_task_page_rick = TASK_CODE;
}

paddr_t mmu_next_free_kernel_page(void) {
	uint32_t pagina_libre = free_kernel_page;
	free_kernel_page += PAGE_SIZE ;
  	return pagina_libre;
}

paddr_t mmu_next_free_task_page(void) {
	uint32_t pagina_libre = free_task_page;
	free_task_page += PAGE_SIZE * 2; 
  	return pagina_libre;
}

vaddr_t mmu_next_free_virtual_task_page_morty(void) {
	uint32_t pagina_libre = free_virtual_task_page_morty;
	free_virtual_task_page_morty += (2 * PAGE_SIZE);
  	return pagina_libre;
}

vaddr_t mmu_next_free_virtual_task_page_rick(void) {
	uint32_t pagina_libre = free_virtual_task_page_rick;
	free_virtual_task_page_rick += (2 * PAGE_SIZE);
  	return pagina_libre;
}


paddr_t mmu_init_kernel_dir(void) {
 	page_dir_entry *page_dir= (page_dir_entry *) KERNEL_PAGE_DIR;
	page_table_entry *page_table_0= (page_table_entry *)KERNEL_PAGE_TABLE_0;
	for(int i = 0;i < 1024;i++){
		page_dir[i]= (page_dir_entry){0};
		page_table_0[i] = (page_table_entry){0};
	}
	page_dir[0].present = 0x1; 
	page_dir[0].user_supervisor=0x0 ; // 0 = supervisor / 1 = usuario
	page_dir[0].read_write = 0x1; 
	page_dir[0].page_table_base =(uint32_t) page_table_0 >> 12; //quiero el address en los 20 bits mas significativos
	for(int i = 0 ; i < 1024 ; i++){
		page_table_0[i].present = 0x1; 
		page_table_0[i].user_supervisor=0x0 ; // 0 = supervisor / 1 = usuario
		page_table_0[i].read_write = 0x1; 
		page_table_0[i].physical_address_base =i; 
	}
	return (paddr_t)page_dir;
}




void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
	//armo los offsets a partir de la dir virtual
	uint32_t dir_index = (virt>>22);
	uint32_t table_index = ((virt<<10)>>22);
	//inicializo la dir table
	page_dir_entry *page_dir = (page_dir_entry*) cr3;
	if (page_dir[dir_index].present == 0) {
		//pido una pagina libre al kernel para cargarle la tabla
		uint32_t nuevapag = mmu_next_free_kernel_page();
		page_table_entry *new_table =(page_table_entry*) nuevapag;
		//asigno los atributos al dir table y prendo el bit present
		page_dir[dir_index].present = 1;
		page_dir[dir_index].read_write = (attrs<<30)>>31;   
		page_dir[dir_index].user_supervisor = (attrs<<29)>>31;
		//enchufo el dir table con la tabla nueva
		page_dir[dir_index].page_table_base = (nuevapag >> 12);
		//inicializo los valores de la tabla en 0
		for (int i = 0; i < 1024; ++i) 
			new_table[i] = (page_table_entry){0};			
				
	}
	//me meto en la tabla
	page_table_entry *page_table = (page_table_entry*)(((uint32_t)((page_dir[dir_index].page_table_base)))<<12);	
	//asigno sus atributos ,le prendo el presente y le enchufo la direccion fisica
	page_table[table_index].present = 1;
	page_table[table_index].read_write = (attrs<<30)>>31;
	page_table[table_index].user_supervisor = (attrs<<29) >> 31;
	page_table[table_index].physical_address_base = (phy>>12);
	
	//hago un call a tlbflush para limpiar la cache de la tlb y cargar los nuevos cambios
	tlbflush();
}


paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {
	vaddr_t dir_index = virt >> 22;
	vaddr_t table_index = (virt << 10) >> 22;

	page_dir_entry *directory=(page_dir_entry *) ((cr3 & ~0xfff)); //me quedo con los bits altosdel cr3
	//accedo al page_dir
	page_dir_entry page_dir = directory[dir_index];
	//accedo a la page_table y cambio el bit presente a 0
	page_table_entry *page_table =(page_table_entry *) (page_dir.page_table_base << 12); //el page table address es de "20 bits" y quiero moverlo a los 20 bits mas altos
	page_table[table_index].present = 0;
	tlbflush();
	return 0;
}

paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start,vaddr_t virt_target, size_t pages){
	
	page_dir_entry *page_dir= (page_dir_entry *) mmu_next_free_kernel_page(); // inicializo un directorio
	page_table_entry *page_table_0= (page_table_entry *)mmu_next_free_kernel_page(); // inicializo su primera tabla
	
	page_dir = (page_dir_entry *)(((uint32_t) page_dir) &~ 0xFFF);
	//inicializo el el directorio y la primera page table
	for(int i = 0;i < 1024;i++){
		page_dir[i]= (page_dir_entry){0};
	}
	page_dir[0].present = 0x1; 
	page_dir[0].user_supervisor=0x0 ; // 0 = supervisor / 1 = usuario
	page_dir[0].read_write = 0x1; 
	page_dir[0].page_table_base =(uint32_t) page_table_0 >> 12; //quiero el address en los 20 bits mas significativos
	for(int i = 0 ; i < 1024 ; i++){
		page_table_0[i].present = 0x1; 
		page_table_0[i].user_supervisor=0x0 ; // 0 = supervisor / 1 = usuario
		page_table_0[i].read_write = 0x1; 
		page_table_0[i].physical_address_base =i; 
	}

	for(uint32_t i = 0; i < pages ; i++){ // Primero mapeo
        	mmu_map_page((uint32_t) page_dir,virt_target + i * PAGE_SIZE, code_start + i * PAGE_SIZE,0x0006);
        	mmu_map_page(0x25000,virt_target + i * PAGE_SIZE, code_start + i * PAGE_SIZE,0x0006);
	}
	uint32_t *src = (uint32_t *) phy_start;
	uint32_t *dest = (uint32_t *) virt_target;
	for(uint32_t i = 0 ; i < 1024 ; i++ ){
		dest[i] = src[i];
	}
	for(uint32_t i = 0 ; i < pages ; i++){
		mmu_unmap_page(0x25000,virt_target + i * PAGE_SIZE);
	}
	return (paddr_t)page_dir;
}
