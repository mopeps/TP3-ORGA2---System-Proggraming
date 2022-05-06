section .text

%include "defines.asm"

global copy_mem
global desalojar_tarea_actual_asm
global breakpoint
extern print_hex
extern desalojar_tarea_actual
extern sched_next_task
extern print_stack_err_trace

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

	
copy_mem:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8]
    mov ebx, [ebp + 12]
    mov ecx, 0
    imul ebx, 4096 
    .loop:
        cmp ecx, ebx
        je .fin
        mov eax, [edx + ecx]
        mov [ecx + 0x01d00000], eax
        add ecx, 4
        jmp .loop
    
    .fin:

        pop ebp
        ret  


breakpoint:
	xchg bx, bx
	ret


desalojar_tarea_actual_asm:
	str cx
	shr ecx,3
	push ecx
	call desalojar_tarea_actual
	add esp,4
    
	.fin:
		jmp GDT_OFF_TSS_IDLE:0
		ret
