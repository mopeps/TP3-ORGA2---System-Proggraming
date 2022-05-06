; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"
%include "defines.asm"
global start

extern IDT_DESC
extern idt_init
extern screen_draw_box 
extern GDT_DESC
extern pic_reset
extern pic_enable
extern mmu_init_kernel_dir
extern mmu_init
extern mmu_map_page
extern mmu_unmap_page
extern mmu_init_kernel_task
extern tss_init
extern sched_init
extern game_init

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

space_line_msg times 80 db  ' '
space_line_len equ  $ - space_line_msg

LU_msg db 'LUs: 675/18, 600/18, 272/19'
LU_len equ  $ - LU_msg

puntos db     '00  01  02  03  04  05  06  07  08  09'
puntos_len equ    $ - puntos
;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    ;breakpoint
    xchg bx, bx
    ; Habilitar A20
    call A20_disable ;si no activamos a20, cualquier direccion de
		     ;memoria que queramos acceder va a estar 
		     ;limitada al primer mega de memoria
    call A20_check
    call A20_enable
    call A20_check 
    ; Cargar la GDT
    lgdt [GDT_DESC]
    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax 
    ; Saltar a modo protegido
    jmp GDT_OFF_CODIGO_0:modo_protegido

BITS 32
modo_protegido:

    ; Establecer selectores de segmentos
    mov ax, GDT_OFF_DATO_0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; Establecer la base de la pila
    mov ebp, EBP_INIT
    mov esp, ESP_INIT 
 
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 10, 10
    
    
    ; Inicializar pantalla
    mov ax, SEGMENTO_OFF_PANTALLA
    mov fs, ax
    ;FONDO VERDE
    push 0xa0
    push 32
    push 80
    push 41
    push 0
    push 0
   ; mov [ebp - 8], 32
   ; mov [ebp - 16], 0xaa
    call screen_draw_box
    add esp,6*4

    ;LINEA NEGRA 
    print_text_pm space_line_msg, space_line_len, 0x00, 0, 0

    	
    ;RED BOX
    push 0x44
    push 32
    push 10
    push 3
    push 5
    push 42
    call screen_draw_box
    add esp,6*4
    
    ;BLUE BOX
    push 0x11
    push 32
    push 10
    push 3
    push 65
    push 42 
    call screen_draw_box
    add esp,6*4
    ;xchg bx, bx
    print_text_pm puntos, puntos_len, 0x01, 43, 20
    print_text_pm puntos, puntos_len, 0x04, 46, 20
     
		 
    ; Inicializar el manejador de memoria
     call mmu_init
    ; Inicializar el directorio de paginas
     call mmu_init_kernel_dir  
    ; Cargar directorio de paginas
     mov eax,KERNEL_PAGE_DIR
     mov cr3,eax
    ; Habilitar paginacion
     mov eax,cr0
     or eax,0x80000000
     mov cr0,eax

     ;hacer la impresion de LU
     print_text_pm LU_msg, LU_len, 0x07, 0, 0  
     ;xchg bx, bx
     ;prueba de mmu map page
     push 2 ; de ecritura
     push 0x00400000 ; direccion fisica
     push 0x0050E020 ; direccion virtual
     mov eax, cr3
     push eax
     call mmu_map_page
     add esp,4*4
     
     mov BYTE [0x0020e027],0x1
     mov BYTE [0x0050e027],0x1
     


     push 0x0050E020
     mov eax,cr3
     push eax
     call mmu_unmap_page
     add esp,4*2

     xchg bx, bx

    ;Inicializar tss  // inicializo idle aca tambien
     call tss_init
     

    ; Inicializar el scheduler
      call sched_init

    ; Inicializar la IDT
      call idt_init

 
    ; Cargar IDT
      lidt [IDT_DESC]

    ; Configurar controlador de interrupciones
      call pic_reset
      call pic_enable
      sti
    ; Cargar tarea inicial
    ;xchg bx, bx
    call game_init

     ;xchg bx, bx
     mov ax,GDT_OFF_TSS_INIT
     ltr ax


     xchg bx,bx
     jmp GDT_OFF_TSS_IDLE:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
