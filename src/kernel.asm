; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic
extern screen_pintar_pantalla



;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; habilitar A20
	call habilitar_A20
    ; cargar la GDT
	
	
	lgdt [GDT_DESC]

	; setear el bit PE del registro CR0
	
	mov eax,cr0 
	or eax,1 
	mov cr0,eax
	
	; pasar a modo protegido
	jmp 18<<3:mp
		
BITS 32
	mp:

	xor eax, eax
	mov ax, 0xa0
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov ss, ax
	mov ax, 22 << 3
	mov fs, ax
		
    ; acomodar los segmentos

    ; setear la pila
	mov esp, 0x27000
	mov ebp, esp
		
	call screen_pintar_pantalla
	
    ; pintar pantalla, todos los colores, que bonito!

    ; inicializar el manejador de memoria

    ; inicializar el directorio de paginas

    ; inicializar memoria de tareas

    ; habilitar paginacion

    ; inicializar tarea idle

    ; inicializar todas las tsss

    ; inicializar entradas de la gdt de las tsss

    ; inicializar el scheduler

    ; inicializar la IDT
	
	call idt_inicializar

    lidt [IDT_DESC]
   
	int 13
   
   
    ; configurar controlador de interrupciones

    ; cargar la tarea inicial

    ; saltar a la primer tarea

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
