; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"


BITS 32


;; PIC
extern fin_intr_pic1
extern printear
extern limpiar_pantalla
extern print_numerito
extern sched_proximo_indice
extern inhabilitar_tarea
extern sched_indice_actual
extern sched_proxima_bandera
extern tareas_arreglo
extern game_navegar
extern game_fondear
extern game_canonear
extern imprimir_bandera
extern sched_bandera_actual

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:

	pushad
	call sched_indice_actual
	push ax
	
	dec byte [habilitadas]
	call inhabilitar_tarea
	pop ax	
	jmp 24<<3:0
	
	popad
	iret
	
    ; To Infinity And Beyond!!
    
   ; call limpiar_pantalla
   ; mov edi, %1
   ; push edi
   ; call printear
   ; add esp, 4
   ; mov eax, 0xFFF2
   ; mov ebx, 0xFFF2
   ; mov ecx, 0xFFF2
   ; mov edx, 0xFFF2
   ; jmp $
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'
offset:  				dd 0x00000000
selector:				dw 0x00000000
habilitadas:			dw 0x00000008
ejecutandoBanderas: 	dw 0x0
pasePorSys: 			dw 0x0

;;
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


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
    pushad
    ;xchg bx, bx
    call fin_intr_pic1
    call sched
    popad
    iret    


;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    
    pushad
    xor eax, eax
    call fin_intr_pic1
    xor eax, eax
    in al, 0x60
    push eax
    call print_numerito
    add esp, 4

    popad
    iret


    
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

global _isr50
_isr50:
    pushad
    call fin_intr_pic1
    xchg bx,bx
    
    .fondear:
    cmp eax, 0x923
    jne .misil
    mov eax, cr3
    push eax
    push ebx 
    call game_fondear
    pop ebx
    pop eax
    jmp 24<<3:0
	jmp .fin
    
    .misil:
    cmp eax, 0x83a
    jne .navegar 
    push ecx
    push ebx    
    call game_canonear    
    pop ebx
    pop ecx    
    jmp 24<<3:0    
    jmp .fin
    
	.navegar:
	cmp eax, 0xaef
	jne .fin	
	push ecx
	push ebx
	mov eax, cr3
	push eax	
	call game_navegar
	pop eax
	pop ebx
	pop ecx	
	jmp 24<<3:0		
	
	.fin:
    popad
    iret



global _isr66
_isr66:
    pushad
    xchg bx, bx
    call fin_intr_pic1
    xor eax, eax
    call sched_bandera_actual
    push eax
    call imprimir_bandera
    pop eax
    jmp 24<<3:0
    popad
    iret





;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
    pushad
    
    inc DWORD [reloj_numero]
    mov ebx, [reloj_numero]
    cmp ebx, 0x4
    jl .ok
        mov DWORD [reloj_numero], 0x0
        mov ebx, 0
    .ok:
        add ebx, reloj
        imprimir_texto_mp ebx, 1, 0x0f, 24, 79

    popad
    ret
    
sched:
    pushad
  
	xchg bx, bx
    cmp word [ejecutandoBanderas], 1
    jne  .ejecutoSigTarea
    call ejecutarBanderas
    
    .ejecutoSigTarea:
	xchg bx, bx
    inc DWORD [reloj_numero]
    mov ebx, [reloj_numero]
    cmp ebx, 0x3
	jle .ok
	
	 ;xchg bx, bx
	jmp .banderas
	
	.banderas:
		
		mov dword [reloj_numero], 0
	 	mov word [ejecutandoBanderas], 1
		call ejecutarBanderas
		jmp .fin
			
    .ok:
		call tareas_arreglo  ; - esto apra debuguear que iban eliminando las tareas adecuadamente
		mov edx, ejecutandoBanderas ;  -- lo mismo
	    ;xchg bx, bx
		call sched_indice_actual
		mov cx, ax
		call sched_proximo_indice
		;xchg bx, bx
		cmp cx, ax
		je .fin
        add ax, 25
        shl ax, 3
        mov [selector], ax
        jmp far [offset]
                
    .fin: 
		popad
		ret

ejecutarBanderas:
	xchg bx, bx
	call sched_proxima_bandera
	mov cx, ax
	cmp cx, -1
	je .finEjecutarBanderas
	
	add ax, 33
	shl ax, 3
	mov [selector], ax
	jmp far [offset]
	
	ret
	
	.finEjecutarBanderas:	
	mov word [ejecutandoBanderas], 0 
	ret
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
