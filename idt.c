/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"
#include "defines.h"
#include "isr.h"
#include "screen.h"

extern void breakpoint(void);

idt_entry_t idt[255] = {0};

idt_descriptor_t IDT_DESC = {sizeof(idt) - 1, (uint32_t)&idt};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/


#define IDT_ENTRY(numero) 									\
    idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);  	\
    idt[numero].segsel = (uint16_t) GDT_OFF_CODIGO_0; 						 	\
    idt[numero].attr = (uint16_t) INTERRUPT_0_ATTR; 							\
    idt[numero].offset_31_16 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);

#define IDT_ENTRY_TASK(numero) 									\
    idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);  	\
    idt[numero].segsel = (uint16_t) GDT_OFF_CODIGO_0; 						 	\
    idt[numero].attr = (uint16_t) INTERRUPT_3_ATTR; 							\
    idt[numero].offset_31_16 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);				


void idt_init() {
  // Excepciones
  IDT_ENTRY(0);
  IDT_ENTRY(1);
  IDT_ENTRY(2);
  IDT_ENTRY(3);
  IDT_ENTRY(4);
  IDT_ENTRY(5);
  IDT_ENTRY(6);
  IDT_ENTRY(7);
  IDT_ENTRY(8);
  IDT_ENTRY(9);
  IDT_ENTRY(10);
  IDT_ENTRY(11);
  IDT_ENTRY(12);
  IDT_ENTRY(13);
  IDT_ENTRY(14);
  IDT_ENTRY(15);
  IDT_ENTRY(16);
  IDT_ENTRY(17);
  IDT_ENTRY(18);
  IDT_ENTRY(19);

  //Reloj
  IDT_ENTRY(32);
    
  //Teclado
  IDT_ENTRY(33);

  //Software
  IDT_ENTRY_TASK(88);
  IDT_ENTRY_TASK(89);
  IDT_ENTRY_TASK(100);
  IDT_ENTRY_TASK(123);
}


void set_debug_flag(uint8_t val){
    debug_flag = val;
}

uint8_t read_debug_flag(){
    return debug_flag;
}

void set_debug_interface_flag(uint8_t val){
    debug_interface_flag = val;
}

uint8_t read_debug_interface_flag(){
    return debug_interface_flag;
}
bool tieneErrorCode(uint32_t interrupcion){
	bool interrupciones[22] = {false, // 0
				   false, // 1
				   false, // 2
				   false, // 3 
				   false, // 4 
				   false, // 5 
				   false, // 6 
				   false, // 7 
				   true , // 8
				   false, // 9
				   true , // 10 
				   true , // 11
				   true , // 12
				   true , // 13 
				   true , // 14
				   false, // 15
				   false, // 16
				   true , // 17
				   false, // 18
				   false, // 19
				   false, // 20
				   true };// 21
		return interrupciones[interrupcion];
}

void debug_init(uint32_t interrupcion,uint32_t eflags,uint32_t ss,uint32_t task,uint32_t gs,uint32_t fs,uint32_t es,uint32_t ds,/*pushad empieza aca*/uint32_t edi,uint32_t esi,uint32_t ebp,uint32_t ESP,uint32_t ebx,uint32_t edx,uint32_t ecx,uint32_t eax/*hasta aca el pushad*/,uint32_t errorCode,uint32_t eip,uint32_t cs){
    set_debug_interface_flag(1);
        if(!tieneErrorCode(interrupcion)){// tengo que arreglar los parametros porque si no hay error code estan desplazados una posicion en la pila
		uint32_t aux = eip;
		eip = errorCode;
		cs = aux;
		errorCode = (uint32_t)-1; // porque el error code puede ser 0
	}
	screen_draw_box(1,20,40,40,32,0x00);
	int initial_y = 5 ;
		print("eax",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(eax,8,27,5,0x02);

		print("ebx",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(ebx,8,27,7,0x02);

		print("ecx",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(ecx,8,27,9,0x02);
		
		print("edx",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(edx,8,27,11,0x02);
		
		print("esi",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(esi,8,27,13,0x02);
		
		print("edi",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(edi,8,27,15,0x02);
		
		print("ebp",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(ebp,8,27,17,0x02);
		
		print("esp",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(ESP,8,27,19,0x02);
		
		print("eip",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(eip,8,27,21,0x02);
		
		print(" cs",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(cs,8,27,23,0x02);
		
		print(" ds",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(ds,8,27,25,0x02);
			
		print(" es",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(es,8,27,27,0x02);
		
		print(" fs",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(fs,8,27,29,0x02);
		
		print(" gs",23,initial_y,0x0f);
		initial_y += 2;
		print_hex(gs,8,27,31,0x02);
		
		print(" ss",23,initial_y,0x0f);
		initial_y += 4;
		print_hex(ss,8,27,33,0x02);
		
		print("eflags",23,initial_y,0x0f);
		initial_y = 7;
		print_hex(eflags,8,30,37,0x02);
		
		print("cr0",47,initial_y,0x0f);
		initial_y += 2;
		print_hex(rcr0(),8,51,7,0x02);
		
		print("cr2",47,initial_y,0x0f);
		initial_y += 2;
		print_hex(rcr2(),8,51,9,0x02);
		
		print("cr3",47,initial_y,0x0f);
		initial_y += 2;
		print_hex(rcr3(),8,51,11,0x02);
		
		print("cr4",47,initial_y,0x0f);
		initial_y += 2;
		print_hex(rcr4(),8,51,13,0x02);
		
		print("err",47,initial_y,0x0f);
		initial_y += 4;
		print_hex(errorCode,8,51,15,0x02);
		
		print("stack",42,initial_y,0x0f);
		initial_y += 8;
		
		print("backtrace",42,initial_y,0x0f);
task++;
		print_stack_err_trace(ESP,ebp,task);

}

bool ebp_valido(uint32_t ebp_bt){
	uint32_t ultima_dir_accesible = (ebp_bt &~ 0xfff) + PAGE_SIZE - 4;
	uint32_t primera_dir_accesible = (ebp_bt &~ 0xfff);

	bool res = 0;

	if(ebp_bt < ultima_dir_accesible && ebp_bt > primera_dir_accesible){
		res = 1;
	}
	return res;
}

void print_stack_err_trace(uint32_t ESP,uint32_t ebp,uint32_t segmento_tarea){
	segmento_tarea -= GDT_IDX_TSS_IDLE;
	//task id
	print_dec(segmento_tarea,2,55,2,0x02);
	//stack
	uint32_t *espointer = (uint32_t *)(ESP);
	uint32_t *esp_de_la_tarea = (uint32_t *)espointer[4];
	print_hex(esp_de_la_tarea[2],8,42,21,0x02); // esp + 8
	print_hex(esp_de_la_tarea[1],8,42,23,0x02); // esp + 4
	print_hex(esp_de_la_tarea[0],8,42,25,0x02); // esp + 0

	//backtrace
	int count = 0;
	uint32_t eip_bt = 0;
	uint32_t *ebp_pointer = (uint32_t *) ebp;

	while (count < 4) {
		breakpoint();
		if(ebp_valido((uint32_t)ebp_pointer)){
			eip_bt = ebp_pointer[1];
			ebp_pointer = (uint32_t *) ebp_pointer[0];
			print_hex(eip_bt,8,42,29 + 2*count, 0x02);
		} else {
			print_hex(0,8,42,29 + 2*count, 0x02);
		}
		count++;
		
	}
		
}
void imprimir_mensaje(uint32_t interrupcion){
        uint32_t size = 1;
        uint32_t n = interrupcion;
        while(n / 10 > 0){
            n = n / 10;
            size++;
        }
	if(read_debug_flag()){
		print("Interrupcion Numero:",25,2,0x02);
       		print_dec(interrupcion, size, 45, 2, 0x02);
	} else {
		print("Interrupcion Numero:",0,0,0xee);
       		print_dec(interrupcion, size, 20, 0, 0xee);

	}

}

void print_scan_code(uint8_t scode){
        uint32_t size = 2;
        if (!(scode & 0x80)) { //Veo si es make o break
                        
            //Imprimo los numeros del 0 al 9
            if(scode == 0x0B) { // Caracter del 0
               size = 1;
               print_dec(0, size, 78, 0, 0xee);
            } else if (scode < 0x0B && scode > 0x00) {
               size = 1;
               print_dec(scode - 1, size, 78, 0, 0xee);
            } else if (scode == 0x15 && debug_flag == 0){ // chequeo si es y (y = 0x15)
                set_debug_flag(1);
            } else if(scode == 0x15 && debug_interface_flag == 1){
				breakpoint();
				reprint_map();
                set_debug_flag(0);
                set_debug_interface_flag(0);
              
            } else if(scode == 0x15 && debug_flag == 1){
                set_debug_flag(0);
            }
        }

}
