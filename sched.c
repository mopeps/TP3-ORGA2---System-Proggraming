/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "game.h"

uint16_t taskQueueRick[256] = {0};// Se almacenan todas las tareas que pertenecen a Rick (son offsets de los descriptores de las tareas en la GDT)
uint16_t taskQueueMorty[256] = {0};// Se almacenan todas las tareas que pertenecen a Morty (son offsets de los descriptores de las tareas en la GDT)
uint16_t len = sizeof(taskList)/sizeof(taskList[0]); // la cantidad total de tareas

extern void desalojar_tarea_actual_asm();


uint16_t haySiguienteTarea(uint16_t arr[], uint16_t from, uint16_t to){  // busca si hay alguna posicion del arreglo,distinto a 0 ,es decir,si todavia queda una tarea
	uint16_t res = 0;
	for(uint16_t i = from; i < to; i++){
		if(arr[i] != 0){
			res = i;
			break;
		}
	}

	return res;
}

void sched_init(void) {
	indice_tarea_rick = 0;
	indice_tarea_morty = 0;

	numero_rick = 0;
	numero_morty = 10;
	
	ultimo_indice_morty = 0;
	ultimo_indice_rick = 0;

	for(int i = 0 ; i < len ; i++){
		if(taskList[i] != 0){
			if(taskList[i] == 1){
				//tss_por_tarea_rick[ultimo_indice_rick] = tss_por_tarea[i];
				taskQueueRick[ultimo_indice_rick] = i << 3;
				ultimo_indice_rick += 1;
			} else {
				//tss_por_tarea_morty[ultimo_indice_morty] = tss_por_tarea[i];													
				taskQueueMorty[ultimo_indice_morty] = i << 3;			
				ultimo_indice_morty += 1;
			}
			
		}
	}
	turno = 1;
}

uint16_t sched_next_task(void) {
	uint16_t proximo = 0;
	turno = turno ^ 1;
	
	if(turno == 0){

		if(taskQueueRick[indice_tarea_rick] == 0){
			proximo = haySiguienteTarea(taskQueueRick, indice_tarea_rick, len);		
			indice_tarea_rick = proximo;
		}
		
		indice_tarea_rick += 1;
		return taskQueueRick[indice_tarea_rick-1];
		
	} else {
		if(taskQueueMorty[indice_tarea_morty] == 0){
			proximo = haySiguienteTarea(taskQueueMorty, indice_tarea_morty, len);
			indice_tarea_morty = proximo;
		} 

		indice_tarea_morty += 1;
		return taskQueueMorty[indice_tarea_morty-1];
		
	}
  	
}

void desalojar_tarea_actual(uint16_t segmento_tarea) {

	uint32_t cr3_viejo = rcr3();
	uint32_t cr3_actual = dir_comienzo_meeseeks[segmento_tarea][3];

	if (turno) {
		if (indice_tarea_morty-1 == 0){
			game_over_excep = true;
			except_from = false;
			return;
			
		} ;//Termino todo

		
		taskQueueMorty[indice_tarea_morty-1] = 0;
	} else {
		if (indice_tarea_rick-1 == 0){
				game_over_excep = true;
				except_from = true;
				return;
		}; //Termino todo


		taskQueueRick[indice_tarea_rick-1] = 0;
	}
	
	// printeo una X donde se desalojo la tarea por una excepcion
	if(pos_meeseks[segmento_tarea][1] != 42069){ 
		print("X", pos_meeseks[segmento_tarea][0], pos_meeseks[segmento_tarea][1] + 1, (C_FG_MAGENTA | C_BG_GREEN));
	}
	pos_meeseks[segmento_tarea][1] = 42069;
	lcr3(cr3_actual);
	mmu_unmap_page(cr3_actual, (vaddr_t)(dir_comienzo_meeseeks[segmento_tarea][0])); //Desmapeo el codigo de la tarea
	mmu_unmap_page(cr3_actual, (vaddr_t)(dir_comienzo_meeseeks[segmento_tarea][0] + PAGE_SIZE)); //Desmapeo la pila de la tarea
	lcr3(cr3_viejo);
	//cambio el estado del simbolo de la tarea
	for(int i = 0 ; i < 25; i++){
		if(estado_tareas[i][0] == segmento_tarea)
			estado_tareas[i][1] = 5;
	}


}

uint32_t add_meeseeks(paddr_t p_codigo, uint32_t x,uint32_t y){
 	if(p_codigo < 0x1D00000 || p_codigo > 0x1D03FFF){
		desalojar_tarea_actual_asm();
	}
	if(x > 80 || y > 40){
		desalojar_tarea_actual_asm();
	}
	
	uint32_t cr3_actual = rcr3();
	uint16_t jugador = 0;
	uint16_t segmento = 0;
	uint32_t virtual_addr = 0;
	bool hayUnaLibre = false;	
	uint8_t numeroMortyTemporal = numero_morty;
	uint8_t numeroRickTemporal = numero_rick;
	uint32_t pila_nivel_0 = 0;
	if (turno) {
		for(int i = 10; i < 20;i++){
			if(estado_tareas[i][1] == 5){
				hayUnaLibre = true;
				segmento = estado_tareas[i][0];
				estado_tareas[i][1] = 0;
				numeroMortyTemporal = i;
			}
		}
		if(!hayUnaLibre && numeroMortyTemporal >= 20){
			return 0;
		}
		if(!hayUnaLibre){
			segmento = GDT_IDX_TSS;
			estado_tareas[numeroMortyTemporal][0] = segmento;
			estado_tareas[numeroMortyTemporal][1] = 0;
			numero_morty++;
		}
		taskQueueMorty[ultimo_indice_morty] = segmento << 3;
		ultimo_indice_morty++;
		if(ultimo_indice_morty == 255){
			ultimo_indice_morty = 1;
		}
		
	
		jugador = 2;
		pos_meeseks[segmento][2] = 2;
		
		

	}  else {
		for(int i = 0; i < 10;i++){
			if(estado_tareas[i][1] == 5){
				hayUnaLibre = true;
				segmento = estado_tareas[i][0];
				estado_tareas[i][1] = 0;
				numeroRickTemporal = i;
			}
		}
		if(!hayUnaLibre && numeroRickTemporal >= 10){
			return 0;
		}
		if(!hayUnaLibre){
			segmento = GDT_IDX_TSS;
			estado_tareas[numeroRickTemporal][0] = segmento;
			estado_tareas[numeroRickTemporal][1] = 0;
			numero_rick++;
		}
		taskQueueRick[ultimo_indice_rick] = segmento << 3;
		ultimo_indice_rick++;
		if(ultimo_indice_rick == 255){
			ultimo_indice_rick = 1;
		}
		jugador = 1;
		pos_meeseks[segmento][2] = 1;

	}
	if(hayUnaLibre){
		virtual_addr = dir_comienzo_meeseeks[segmento][0];
		pila_nivel_0 = dir_comienzo_meeseeks[segmento][4];
	}
	else{	

		virtual_addr = turno ? mmu_next_free_virtual_task_page_morty() : mmu_next_free_virtual_task_page_rick();
		pila_nivel_0 = mmu_next_free_kernel_page();
	}
	uint32_t phy_addr = 0x400000 + (x + y * 80) * 0x2000; 
	pos_meeseks[segmento][0] = x;
	pos_meeseks[segmento][1] = y;
	meeseeks_move_clock[segmento] = false;
	pos_meeseks[segmento][3] = 7;
	pos_meeseks[segmento][4] = 0;

	
	dir_comienzo_meeseeks[segmento][0] = virtual_addr;
	dir_comienzo_meeseeks[segmento][1] = phy_addr;
	dir_comienzo_meeseeks[segmento][2] = p_codigo;
	dir_comienzo_meeseeks[segmento][3] = cr3_actual;
	dir_comienzo_meeseeks[segmento][4] = pila_nivel_0;
	for(uint32_t i = 0; i < 2; i++){ // Primero mapeo
       		 mmu_map_page(cr3_actual,virtual_addr + i * PAGE_SIZE, phy_addr + i * PAGE_SIZE, 0x0006);
	}

	uint32_t *src=(uint32_t *) p_codigo;
	uint32_t *dest =(uint32_t *) virtual_addr;
	for(uint32_t i = 0 ; i < 1024;i++){
		dest[i] = src[i];
	}
	
	tss_por_tarea[segmento] = tss_meeseeks_init(cr3_actual, virtual_addr,pila_nivel_0);
	set_tss_gdt_meeseeks(&tss_por_tarea[segmento], jugador,segmento);
	
	printEstadoTareas();
	return virtual_addr;
}
	
