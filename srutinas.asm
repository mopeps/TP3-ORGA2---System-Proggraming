BITS 32

section .text


%define negro (0x00)
%define verde (0xAA)
%define azul (0x11)
%define rojo (0x44)
%define blanco (0xFF)

screen_draw:
	pushad
	call area_verde



area_verde:
	
