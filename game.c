/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "prng.h"
#include "screen.h"
#include "i386.h"
#include "defines.h"

extern void breakpoint();
extern void desalojar_tarea_actual_asm();

char estados_simbolos_tarea[] = "|/-  X";


void game_init(void) {
    for(int i = 0 ; i < TOTAL_SEMILLAS; i ++){
	    estado_tareas[i][0] = 0;
    }

    for(int i = 0 ; i < TOTAL_SEMILLAS ; i++ ){	
	    pos_semillas[i][0] = 42069;
	    pos_semillas[i][1] = 42069;
    }
    uint32_t rand_X;
    uint32_t rand_Y;
    print("M",17,43,0x04);
    print("R",60,43,0x01);
	estados_simbolos_tarea[3] = (char)134;
	rick_score = 0;
	morty_score = 0;
        cant_semillas = rand() % TOTAL_SEMILLAS;
	print_scores();

    for(int i = 0; i < cant_semillas; i++) {
        rand_X = rand() % SIZE_M;
        rand_Y = rand() % SIZE_N;  
        pos_semillas[i][0] = rand_X;
        pos_semillas[i][1] = rand_Y;
        print("S", pos_semillas[i][0], pos_semillas[i][1] + 1, (C_FG_LIGHT_BROWN | C_BG_GREEN));
    }
  //  breakpoint();
}

void printEstadoTareas(void){
	for(int i = 0 ; i < 10 ; i++){
		if(estado_tareas[i][0] != 0){
			printSingleChar(estados_simbolos_tarea[estado_tareas[i][1]],20+i*4,44,0x07);
		}
	}
	for(int i = 10 ; i < 20 ; i++){
		if(estado_tareas[i][0] != 0){
			printSingleChar(estados_simbolos_tarea[estado_tareas[i][1]],20+(i-10)*4,47,0x07);
		}
	}
	printSingleChar(estados_simbolos_tarea[estado_tareas[23][1]],17,44,0x07);
	printSingleChar(estados_simbolos_tarea[estado_tareas[24][1]],60,44,0x07);
}
void avanzarSimbolo(uint16_t segmento_tarea){
	for(int i = 0 ; i < 20 ; i++){
		if(estado_tareas[i][0] == segmento_tarea && turno == 1){
			estado_tareas[i][1]++;
			estado_tareas[i][1] %= 3;
			estado_tareas[23][1] ++;
			estado_tareas[23][1] %= 3;

		}
		else if(estado_tareas[i][0] == segmento_tarea && turno == 0){
			estado_tareas[i][1]++;
			estado_tareas[i][1] %= 3;
			estado_tareas[24][1] ++;
			estado_tareas[24][1] %= 3;
		}
	}
}

uint8_t aparecio_sobre_semilla(uint32_t pos_x,uint32_t pos_y){
	uint8_t res = 0;
	bool sobreSemilla = false;
	int i = 0;
	while(!sobreSemilla && i < TOTAL_SEMILLAS){
		if(pos_x == pos_semillas[i][0] && pos_y == pos_semillas[i][1]){
			sobreSemilla = true;
			pos_semillas[i][0] = 42069;
			pos_semillas[i][1] = 42069;
		}
		i++;
	}
	if(sobreSemilla){
		print("E", pos_x, pos_y + 1 , (C_FG_CYAN | C_BG_GREEN)); //Piso la semilla
		sumar_puntos(425); 
		cant_semillas--;
		res = 1;
	}
	return res;
}

int abs(int number){
	int res = number;
	if(number < 0){
		res = number * (-1);
	} 
	return res;

}

int distancia_e(int x[], int y[]){
	return (abs(x[0] - y[0]) + abs(x[1] - y[1]));  
}


int bad_end(){
	if((!turno && indice_tarea_rick == 1) || (turno && indice_tarea_morty == 1)) return -1;
	return 0;
}

int* look_seed(uint16_t segmento_tarea){
	int* masCercana = (int*)pos_semillas[0];
	int distancia = 0;
	int min_distancia = 999;
	for(int i = 0 ; i < TOTAL_SEMILLAS ; i++){
		if(pos_semillas[i][1] != 42069){
			distancia = abs(pos_meeseks[segmento_tarea][0] - pos_semillas[i][0]) + abs(pos_meeseks[segmento_tarea][1] - pos_semillas[i][1]);
			if(distancia < min_distancia){
				masCercana = (int*)pos_semillas[i];
				min_distancia = distancia;
			}
		}
			
	}
	return masCercana; // [x, y]
}

int* calcular_desplazamiento(int* pos_semilla, uint16_t segmento_tarea){
	int des_x = pos_semilla[0] - pos_meeseks[segmento_tarea][0];
	int des_y = pos_semilla[1] - pos_meeseks[segmento_tarea][1];

	static int res[2];
	res[0] = des_x;
	res[1] = des_y;

	return res;
}


void print_scores(){
	print_dec(morty_score, 8, 6, 43, (C_FG_WHITE | C_BG_RED));
	print_dec(rick_score, 8, 66, 43, (C_FG_WHITE | C_BG_BLUE));
}

void sumar_puntos(uint32_t puntos){
	if(!turno){
		rick_score += puntos;
	} else {
		morty_score += puntos;
	}
	
	print_scores();
}


void mover_codigo_meeseks(uint16_t segmento_tarea){
		uint32_t virtual_addr = dir_comienzo_meeseeks[segmento_tarea][0];
		uint32_t vieja_phy_addr = dir_comienzo_meeseeks[segmento_tarea][1];
		uint32_t cr3_viejo = rcr3();
		uint32_t cr3_actual = dir_comienzo_meeseeks[segmento_tarea][3];
		lcr3(KERNEL_PAGE_DIR);
		
		uint32_t phy_addr = 0x400000 + (pos_meeseks[segmento_tarea][0] + pos_meeseks[segmento_tarea][1] * 80) * 0x2000; 
		
		dir_comienzo_meeseeks[segmento_tarea][1] = phy_addr;
	
		for(uint32_t i = 0; i < 2; i++){ // Primero mapeo
			mmu_map_page(KERNEL_PAGE_DIR,virtual_addr + i * PAGE_SIZE, phy_addr + i * PAGE_SIZE, 0x0006);
			mmu_map_page(KERNEL_PAGE_DIR,vieja_phy_addr + i * PAGE_SIZE, vieja_phy_addr + i * PAGE_SIZE, 0x0006);
		}
		
		uint32_t *src_p=(uint32_t *) vieja_phy_addr; 
		uint32_t *dest =(uint32_t *) virtual_addr;
		for(uint32_t i = 0 ; i < 2048; i++){
			dest[i] = src_p[i];
		}
		for(uint32_t i = 0; i < 2; i++){ 
			mmu_unmap_page(KERNEL_PAGE_DIR, virtual_addr + i * PAGE_SIZE);
			mmu_unmap_page(KERNEL_PAGE_DIR, vieja_phy_addr + i * PAGE_SIZE);
		}

		lcr3(cr3_actual);
		for(uint32_t i = 0; i < 2; i++){ // Primero mapeo
			mmu_unmap_page(cr3_actual, virtual_addr + i * PAGE_SIZE);
			mmu_map_page(cr3_actual,virtual_addr + i * PAGE_SIZE, phy_addr + i * PAGE_SIZE, 0x0006);
		}
		
	

		lcr3(cr3_viejo);
}


bool estaSobreSemilla(uint16_t segmento_tarea){
    bool res = false;
    for(int i = 0 ; i < TOTAL_SEMILLAS ; i++){
	    if(pos_semillas[i][1] != 42069){
        	if(pos_meeseks[segmento_tarea][0] == pos_semillas[i][0]  && pos_meeseks[segmento_tarea][1] == pos_semillas[i][1]){
            		res = true;
            		pos_semillas[i][0] = 42069;
			pos_semillas[i][1] = 42069;
        	}
    	}
    }
    return res;
}

bool estaSobreContrario(uint16_t segmento_tarea) {
    bool res = false;
    for(uint16_t i = 0; i < GDT_IDX_TSS; i++){
        if((int)pos_meeseks[i][2] == abs((int)pos_meeseks[segmento_tarea][2] - 3)){
            if(pos_meeseks[segmento_tarea][0] == pos_meeseks[i][0]  && pos_meeseks[segmento_tarea][1] == pos_meeseks[i][1]){
                res = true;
            }
        }
    }
    return res;
}

uint32_t desplazar_meeseeks(int des_x, int des_y, uint16_t segmento_tarea){
		
	int limite_des = pos_meeseks[segmento_tarea][3];
	int32_t nuevo_x =(int32_t) pos_meeseks[segmento_tarea][0];
	int32_t nuevo_y =(int32_t) pos_meeseks[segmento_tarea][1];

	// Calculo la nueva cordenada x del meeseeks
	
	if(abs(des_x) + abs(des_y) <= limite_des){
		 nuevo_x += des_x;
		 nuevo_y += des_y;
	}	
	print("X", pos_meeseks[segmento_tarea][0], pos_meeseks[segmento_tarea][1] + 1, (C_FG_GREEN | C_BG_GREEN));
	
	

	nuevo_x = nuevo_x < 0 ? nuevo_x + 80 : nuevo_x % 80;
	pos_meeseks[segmento_tarea][0] = (uint32_t) nuevo_x;
	
	nuevo_y = nuevo_y < 0 ? nuevo_y + 40 : nuevo_y % 40;
	pos_meeseks[segmento_tarea][1] = (uint32_t) nuevo_y;
	
	bool sobreSemilla = estaSobreSemilla(segmento_tarea);
	if(sobreSemilla){
		print("E", nuevo_x, nuevo_y+ 1, (C_FG_CYAN | C_BG_GREEN)); //Piso la semilla
		pos_meeseks[segmento_tarea][1] = 42069; //Para que no lo imprima mas
		pos_meeseks[segmento_tarea][2] =  0;
		//sumo puntos y desalojo la tarea
		sumar_puntos(425); 
		cant_semillas--;
		desalojar_tarea_actual_asm();
		
	}
	else{
		//REIMPRIMO CON LA POSICION NUEVA
		print_meeseek(segmento_tarea);
		//REDUZCO LA CANTIDAD DE PASOS MAXIMO:
		if(meeseeks_move_clock[segmento_tarea]){
			pos_meeseks[segmento_tarea][3] = pos_meeseks[segmento_tarea][3] == 0 ? 1 : pos_meeseks[segmento_tarea][3] - 1;
			meeseeks_move_clock[segmento_tarea] = false;
		}else{
			meeseeks_move_clock[segmento_tarea] = true;
		}
		

		//MUEVO EL CODIGO
		mover_codigo_meeseks(segmento_tarea);
	}
	return 1; 
}


void usar_portal_gun(uint16_t segmento_tarea){
    if(!pos_meeseks[segmento_tarea][4]){
        uint32_t jugador = pos_meeseks[segmento_tarea][2];
        uint32_t jugador_contrario = abs((int)jugador - 3);

        uint16_t contarios_activos[10] = {0};
        uint16_t cant_cont_activos = 0;

        for(uint16_t i = 0; i < GDT_IDX_TSS; i++){
            if(pos_meeseks[i][2] == jugador_contrario){
                contarios_activos[cant_cont_activos] = i;
                cant_cont_activos++;
            }
        }

        uint16_t segmento_contrario = 0;

        if(cant_cont_activos > 0){
            segmento_contrario = contarios_activos[rand() % cant_cont_activos];

            uint32_t rand_X = rand() % SIZE_M;
            uint32_t rand_Y = rand() % SIZE_N;

            //Piso la posicion anterior
            print("X", pos_meeseks[segmento_contrario][0], pos_meeseks[segmento_contrario][1] + 1, (C_FG_GREEN | C_BG_GREEN));

            pos_meeseks[segmento_contrario][0] = rand_X;
            pos_meeseks[segmento_contrario][1] = rand_Y;

            print_meeseek(segmento_contrario);

            pos_meeseks[segmento_tarea][4] = 1;

            mover_codigo_meeseks(segmento_contrario);

        }
    }
}

bool check_game_over(void){

	bool res = false;
	if(cant_semillas <= 0 || game_over_excep){
		res = true;
	}
	return res;
}

void game_over(void){

	char* ganador = " ";
	uint16_t attr = 0;
	if((rick_score > morty_score && !game_over_excep) || (game_over_excep && !except_from)){
		ganador = "GANADOR RICK - SOS BUENARDO";
		attr = (C_FG_BLUE | C_BG_BLACK);

	}else if((rick_score < morty_score && !game_over_excep) || (game_over_excep && except_from)){
		ganador = "GANADOR MORTY - SOS BUENARDO";
		attr = (C_FG_RED | C_BG_BLACK);

	}else{
		ganador = "EMPATE - SON MALARDOS LOS 2";
		attr = (C_FG_GREEN | C_BG_BLACK);
	}

	

	screen_draw_box(0,0,41,80,' ',0x00);
	print(ganador, 26,20,attr);
}
