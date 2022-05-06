/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "screen.h"

extern void breakpoint();

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  for (i = 0; text[i] != 0; i++) {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
    }
  }
}

void printSingleChar(const char elChar,uint32_t x, uint32_t y, uint16_t attr){
	ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
	p[y][x].c = (uint8_t) elChar;
	p[y][x].a = (uint8_t) attr;
}
//printea todos
void print_meeseks(){
	for(int i = 0 ; i < 40 ; i++){
		if( pos_meeseks[i][1] != 42069 ){ 
			if( pos_meeseks[i][2] == 2)
				print("M",pos_meeseks[i][0],pos_meeseks[i][1] + 1  ,0x24);
			else if(pos_meeseks[i][2] == 1)
				print("R",pos_meeseks[i][0],pos_meeseks[i][1] + 1  ,0x21);
		}
	}
}
//printea uno
void print_meeseek(uint32_t meeseek){
			if( pos_meeseks[meeseek][2] == 2)
				print("M",pos_meeseks[meeseek][0],pos_meeseks[meeseek][1] + 1 ,0x24);
			else if(pos_meeseks[meeseek][2] == 1)
				print("R",pos_meeseks[meeseek][0],pos_meeseks[meeseek][1] + 1 ,0x21);
		
	
}

void reprint_map(){
   screen_draw_box(1, 0, 40, 80, 32, (C_FG_GREEN | C_BG_GREEN));
   for(int i = 0; i < 20; i++){
    if(pos_semillas[i][1] != 42069){
        print("S",pos_semillas[i][0],pos_semillas[i][1] + 1,(C_FG_LIGHT_BROWN | C_BG_GREEN));
    }
  }
  breakpoint();
  print_meeseks();
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++) {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++) {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++) {
    for (c = cInit; c < cInit + cSize; c++) {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}
