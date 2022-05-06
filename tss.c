/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_CODIGO_0 (GDT_IDX_CODIGO_0 << 3)
#define GDT_OFF_DATO_0 (GDT_IDX_DATO_0 << 3)
#define GDT_OFF_CODIGO_3 (GDT_IDX_CODIGO_3 << 3)
#define GDT_OFF_DATO_3 (GDT_IDX_DATO_3 << 3)
#define SEGMENTO_OFF_PANTALLA (SEGMENTO_IDX_PANTALLA << 3)
#define GDT_OFF_TSS_INIT  (GDT_IDX_TSS_INIT << 3)
#define GDT_OFF_TSS_IDLE  (GDT_IDX_TSS_INIT << 3)


*/

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"
#include "sched.h"

tss_t tss_initial = {0};
tss_t tss_idle;
tss_t tss_rick;
tss_t tss_morty;
tss_t tss_meeseeks[20];




void tss_idle_init(void){
	//inicializamos los valores  de la task idle
    tss_idle.ss = GDT_OFF_DATO_0; //| GDT_RPL_DATO_0; pondria el or,pero ...RPL_DATO_0 = 0x00...
    tss_idle.ds = GDT_OFF_DATO_0;//| GDT_RPL_DATO_0;
    tss_idle.es = GDT_OFF_DATO_0; //| GDT_RPL_DATO_0; 
    tss_idle.fs = SEGMENTO_OFF_PANTALLA; //| GDT_RPL_DATO_0;
    tss_idle.gs = GDT_OFF_DATO_0; //| GDT_RPL_DATO_0; 
    tss_idle.cs = GDT_OFF_CODIGO_0; //| GDT_RPL_CODIGO_0; 
    tss_idle.cr3 = KERNEL_PAGE_DIR;//la direccion del cr3 del kernel
    tss_idle.eip = TASK_IDLE_CODE;
    tss_idle.ebp = KERNEL_STACK; 
    tss_idle.esp = KERNEL_STACK;
    tss_idle.eflags = DEFAULT_FLAGS_VALUES;// 0x00000202 
    tss_idle.iomap = 0x0000;
    tss_idle.ss0 = GDT_OFF_DATO_0 ;
    tss_idle.esp0 = KERNEL_STACK;

    gdt[GDT_IDX_TSS_IDLE].base_15_0 = (uint32_t)&tss_idle; 
    gdt[GDT_IDX_TSS_IDLE].base_23_16 = (uint32_t)&tss_idle >> 16;
    gdt[GDT_IDX_TSS_IDLE].base_31_24 = (uint32_t)&tss_idle >> 24;

}


void set_tss_gdt_meeseeks(tss_t *task, uint16_t jugador,uint16_t segmento){	//seteamos su descriptor
    gdt[segmento].base_15_0 = (uint32_t)task; 
    gdt[segmento].base_23_16 = (uint32_t)task >> 16;  
    gdt[segmento].base_31_24 = (uint32_t)task >> 24;  
    gdt[segmento].limit_15_0 =sizeof(tss_t)-1;
    gdt[segmento].type	= 0x9;
    gdt[segmento].s		= 0x00;
    gdt[segmento].dpl	= 0x00;
    gdt[segmento].p		= 0x1;
    gdt[segmento].limit_19_16 = 0x00;
    gdt[segmento].avl	= 0x0;
    gdt[segmento].l		= 0x0;
    gdt[segmento].db		= 0x1;
    gdt[segmento].g		= 0x00;


    taskList[segmento] = jugador; 

    cantidadDeTareas++;
    if(segmento == GDT_IDX_TSS){
    	GDT_IDX_TSS++;
    }

}


tss_t tss_generic_init( paddr_t p_start, paddr_t code_start,vaddr_t v_start, size_t pages){
    tss_t parcial_tss;

    paddr_t newCR3 = mmu_init_task_dir(p_start,code_start,v_start,pages);

    parcial_tss.ss = GDT_OFF_DATO_3 | GDT_RPL_DATO_3;
    parcial_tss.ds = GDT_OFF_DATO_3 | GDT_RPL_DATO_3;
    parcial_tss.es = GDT_OFF_DATO_3 | GDT_RPL_DATO_3; 
    parcial_tss.fs = GDT_OFF_DATO_3 | GDT_RPL_DATO_3;
    parcial_tss.gs = GDT_OFF_DATO_3 | GDT_RPL_DATO_3; 
    parcial_tss.cs = GDT_OFF_CODIGO_3 | GDT_RPL_CODIGO_3; 
    parcial_tss.cr3 = newCR3;
    parcial_tss.eip = v_start;
    parcial_tss.ebp = v_start + pages * PAGE_SIZE;
    parcial_tss.esp = v_start + pages * PAGE_SIZE;
    parcial_tss.eflags = DEFAULT_FLAGS_VALUES;// 0x00000202 
    parcial_tss.iomap = 0x0000;
    parcial_tss.ss0 = GDT_OFF_DATO_0;
    parcial_tss.esp0 = mmu_next_free_kernel_page() + PAGE_SIZE;
    parcial_tss.ldt = 0;



    return parcial_tss;
    
} 

tss_t tss_meeseeks_init(uint32_t cr3 ,vaddr_t v_start,uint32_t pila_nivel_0){
    tss_t parcial_tss;

    parcial_tss.ss = GDT_OFF_DATO_3 | GDT_RPL_DATO_3;
    parcial_tss.ds = GDT_OFF_DATO_3 | GDT_RPL_DATO_3;
    parcial_tss.es = GDT_OFF_DATO_3 | GDT_RPL_DATO_3; 
    parcial_tss.fs = GDT_OFF_DATO_3 | GDT_RPL_DATO_3;
    parcial_tss.gs = GDT_OFF_DATO_3 | GDT_RPL_DATO_3; 
    parcial_tss.cs = GDT_OFF_CODIGO_3 | GDT_RPL_CODIGO_3; 
    parcial_tss.cr3 = cr3;
    parcial_tss.eip = v_start;
    parcial_tss.ebp = v_start + 2 * PAGE_SIZE;
    parcial_tss.esp = v_start + 2 * PAGE_SIZE;
    parcial_tss.eflags = DEFAULT_FLAGS_VALUES;// 0x00000202 
    parcial_tss.iomap = 0x0000;
    parcial_tss.ss0 = GDT_OFF_DATO_0;
    parcial_tss.esp0 = pila_nivel_0 + PAGE_SIZE;
    parcial_tss.ldt = 0;



    return parcial_tss;
    
} 


void tss_init(void) {
    gdt[GDT_IDX_TSS_INIT].base_15_0 = (uint32_t)&tss_initial; 
    gdt[GDT_IDX_TSS_INIT].base_23_16 = (uint32_t)&tss_initial >> 16;
    gdt[GDT_IDX_TSS_INIT].base_31_24 = (uint32_t)&tss_initial >> 24;

    int len = sizeof(taskList)/sizeof(taskList[0]);
    for(int i = 0 ; i < len ; i++){
            taskList[i] = 0;
    }

    cantidadDeTareas = 0;

    GDT_IDX_TSS = GDT_IDX_TSS_IDLE + 1;


    //Reservo espacio para la pila nivel 3
    

    tss_idle_init();

    
    tss_por_tarea[GDT_IDX_TSS] = tss_generic_init( 0x10000, 0x1D00000,0x1D00000, 4);
    estado_tareas[23][0] = GDT_IDX_TSS;
    set_tss_gdt_meeseeks(&tss_por_tarea[GDT_IDX_TSS], 1, GDT_IDX_TSS);
    
    tss_por_tarea[GDT_IDX_TSS] = tss_generic_init( 0x14000, 0x1D04000,0x1D00000, 4);
    estado_tareas[24][0] = GDT_IDX_TSS;
    set_tss_gdt_meeseeks(&tss_por_tarea[GDT_IDX_TSS], 2, GDT_IDX_TSS);
    
    
}
