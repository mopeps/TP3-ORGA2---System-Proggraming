; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"
%include "defines.asm"


BITS 32

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

pos_x: db 0xFF
pos_y: db 0xFF
meeseeks_virtual_addr: dd 0xffffffff

;; PIC
extern pic_finish1

;; Sched
extern sched_next_task
extern imprimir_mensaje
extern print_scan_code

extern desalojar_tarea_actual
extern desalojar_tarea_actual_asm
extern add_meeseeks
extern print_meeseks
extern aparecio_sobre_semilla
extern desplazar_meeseeks
extern look_seed
extern bad_end
extern calcular_desplazamiento
extern mmu_next_free_task_page
extern usar_portal_gun
extern read_debug_flag
extern read_debug_interface_flag
extern debug_init
extern check_game_over
extern game_over
extern printEstadoTareas
extern avanzarSimbolo

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:

 
    ;xchg bx,bx
    mov edi,[esp+12] ; movemos los eflags
    pushad
    call read_debug_flag
    cmp al, 0
    je .desalojo

    
    .debugger:
        xor eax,eax
        mov ax,ds
        push eax
    
        mov ax,es
        push eax
        
        mov ax,fs
        push eax
        
        mov ax,gs
        push eax
        
        str cx
        shr ecx,3
        push ecx
        
        xor eax,eax
        mov ax,ss
        push eax
        
        push edi
        mov eax,%1
        push eax
        call debug_init
        add esp,4*8

    ;xchg bx, bx

    .desalojo: 
    

        ;.sigue:
    	mov eax, %1
    	push eax
    	call imprimir_mensaje
    	add esp, 4
        call desalojar_tarea_actual_asm

    
    popad
    iret

%endmacro

global _isr32
global _isr33
global _isr88
global _isr89
global _isr100
global _isr123

_isr32:
    pushad

    ;xchg bx,bx
    call pic_finish1

    call next_clock
    call printEstadoTareas
    call read_debug_interface_flag
    cmp al,0
    je .checkGameOver
    jmp .fin 


    .checkGameOver:
    call check_game_over
    cmp eax,0
    je .siga

    call game_over
    jmp $

    .siga:


    call sched_next_task
        
    cmp ax, 0
    je .fin

    mov word [sched_task_selector], ax
    jmp far [sched_task_offset] 

    .fin:
        popad
        iret    

_isr33:
    pushad
    in al, 0x60 ; Atajo la tecla
    xchg bx, bx
    push eax
    call print_scan_code
    add esp, 4
    call pic_finish1
    popad
    iret

_isr88:
    pushad
    ;eax: puntero a codigo
    ;ebx: columna en el mapa
    ;ecx: fila en el mapa
    
    mov edi,ecx ; y
    mov esi,eax ; puntero al codigo
    
    push edi
    push ebx
    call aparecio_sobre_semilla
    add esp,4 * 2

    cmp al,0
    mov DWORD [meeseeks_virtual_addr],0
    jne .idle
    
    mov ecx,edi
    mov eax,esi
    mov esi,ebx
    
    ;xchg bx, bx
    push edi ;y
    push esi ;x 
    push eax
    call add_meeseeks
    add esp, 4*3

    mov [meeseeks_virtual_addr],eax
    call print_meeseks
    
    ;xchg bx, bx
     

    .idle:
    jmp GDT_OFF_TSS_IDLE:0
    

    popad
    mov eax,[meeseeks_virtual_addr]
    iret

_isr89:
    pushad
    ;mov eax, 0x59
    ;xchg bx,bx
    str cx
    shr ecx, 3
    push ecx
    call usar_portal_gun
    add esp, 4
    
    str cx
    shr ecx,3
    push ecx
    call avanzarSimbolo
   add esp,4


    jmp GDT_OFF_TSS_IDLE:0

    popad
    iret

_isr100:
    pushad
    ;eax, desplazamiento en x
    ;ebx, desplazamiento en y
    mov esi, eax
    mov edi, ebx

    call bad_end
    cmp eax, 0  
    jne .bad_end ;Si en el eax hay un -1 salto tambien a bad_end y completo el ebx
    
    str cx
    shr ecx, 3
    push ecx
    call look_seed
    add esp, 4
    

   ;xchg bx,bx

    str cx
    shr ecx, 3
    push ecx
    push eax
    call calcular_desplazamiento
    add esp, 4*2
  
    mov ebx, [eax + 4]
    mov eax,[eax]

   ; xchg bx,bx
    mov [pos_x], al
    mov [pos_y], bl
    
   

    jmp .fin


    .bad_end:
    mov ebx, eax ;Muevo el otro -1 al ebx
    

    .fin:
        popad
        mov al, [pos_x]
        mov bl, [pos_y]

        jmp GDT_OFF_TSS_IDLE:0
        iret

_isr123:

    ;eax: deplazamiento en x
    ;ebx: desplazamiento en y
    pushad

    mov edi, eax

    call bad_end
    cmp eax, 0
    jne .desalojo
    
    ;xchg bx, bx

    str cx
    shr ecx, 3
    push ecx
    push ebx
    push edi
    call desplazar_meeseeks
    add esp, 4*3
    
   ; xchg bx, bx

    str cx
    shr ecx,3
    push ecx
    call avanzarSimbolo
    add esp,4
    
    jmp .fin

    ;mov eax, 0x7b

    .desalojo:
    	xchg bx,bx
        call desalojar_tarea_actual_asm

    .fin:
        jmp GDT_OFF_TSS_IDLE:0
        popad
        iret

;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
