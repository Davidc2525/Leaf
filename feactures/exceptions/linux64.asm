


; Manejador de excepciones  para linux 64
; autor: Colmenares David, david25pcxtreme@gmail.com

%ifndef _H_
%define _H_
; activa debug, para comprobacion en desarrollo
%define DEBUG 0
; evita la expacion de codigo de comprobacion en algunos casos
%define FAST 0
; tamaño maximo de la pila de excepciones
%define EXCEPTION_MAX 100
extern printf
extern malloc
extern calloc
extern free
extern exit
; global _list_find_h_by_type

; list to save handlers-----
; void * new_list(int size)
%macro new_list 1
    mov rdi, 3+(%1)
    mov rsi, 8
    call calloc
    mov qword [rax],%1   ; store size by self
    
    ;contexto
    mov [rax+8],rbp
    mov [rax+16],rsp
    ;contexto
    
    test rax,rax
    je h_malloc_error
%endmacro

; void list_set(void * list,int index, Handler * value)
%macro list_set 3
  mov rdi,%1
  mov rsi,%2
  mov rdx,%3
  call _list_set
  ; mov dword [%1+4+(%2*4)],%3
%endmacro

; %define list_set(list,index,value) mov dword [list+(index*4)],value

; void * list_get(void * list,int index)
%macro list_get 2
    mov  rax, [%1+24+(%2*8)]
%endmacro

; int list_size(void * list)
%macro list_size 1
   mov rax, [%1]
%endmacro

; Handler * list_find_h_by_type (void*list,int type)
%macro list_find_h_by_type 2
    mov rdi,%1
    mov rsi,%2
    call _list_find_h_by_type
%endmacro


; --------------------
; class Handler {
; int type;
; long rutine;
; }
; %define H_SIZE     16    ; no property
%define H_TYPE(o)  [o]   ; 8 bytes int
%define H_RUTINE(o)[o+8] ; 8 bytes long

;void J_H_RUTINE(Handler*h)
%macro J_H_RUTINE 1
	  ; salto a la rutina del manejador (catch)
    jmp H_RUTINE(%1)
%endmacro


%macro new_handler 0
    mov rdi,2
    mov rsi,8
    call calloc
    test rax,rax
    je h_malloc_error
%endmacro

%macro new_handler 2
    new_handler
    mov qword H_TYPE(rax),%1
    mov qword H_RUTINE(rax),%2
%endmacro

%macro delete_handler 1
    mov rdi, %1
    call free
%endmacro

; -------------------

%macro _push_ 1
    mov  rdi,%1
    call _push_map_exception
%endmacro

%macro _pop_ 0
    call _pop_map_exception
%endmacro

%macro  multipush 1-*

  %rep  %0
        push    %1
  %rotate 1
  %endrep

%endmacro
%macro  multipop 1-*

  %rep %0
  %rotate -1
        pop     %1
  %endrep

%endmacro


section .bss
    align 8
    exception_stack resd EXCEPTION_MAX
    ; inst1 resb 12
section .data
    align 8
    exception_count_stack dd 0
    cc dd 0
    msg db '%d',10, 0
    msg_dump_stack_head db "stack -> [",0
    msg_dump_stack_item db " %ld ",0
    msg_dump_stack_foot db "]",10,0
    msg_malloc_error db "error al reservar memoria",10,0
    msg_no_found db "ERROR: no se encotro este manejador (bloque catch) para el tipo: %d.",10,0

section .text
    align 8
msg_stack_overflow: db "exception_stack overflow, stack size: %d, stack index: %d",10,0
_push_map_exception:
    ; _push:
    push rbx
    %if !FAST
      cmp dword [exception_count_stack],EXCEPTION_MAX
      je .stack_overflow
    %endif
    mov rbx,   [exception_count_stack]
    mov        [exception_stack+(rbx*8)],rdi
    inc dword  [exception_count_stack]
    pop rbx
    ret

%if !FAST
.stack_overflow:
    mov rdi, msg_stack_overflow
    mov rsi, EXCEPTION_MAX
    mov rdx,  [exception_count_stack]
    mov rax, 0
    call printf
    mov rdi,1
    call exit
%endif

msg_stack_underflow: db "exception_stack underflow",10,0
_pop_map_exception:
    ; _pop:
    push rbx
    %if !FAST
      cmp dword [exception_count_stack],0
      je .stack_underflow
    %endif
    dec dword [exception_count_stack]
    mov rbx,  [exception_count_stack]
    mov rax,  [exception_stack+(rbx*8)]
    mov dword [exception_stack+(rbx*8)],0
    pop rbx
    ret
%if !FAST
.stack_underflow:
    mov rdi, msg_stack_underflow
    mov rax, 0
    call printf

    call _main_handler_

%endif

; helpers
msg_out_bound: db "Indice mayor al tamaño de la lista: index(%d) -> list_size(%d)",10,0
list_index_out_bound:
mov rdx,rdi
mov rdi,msg_out_bound
mov rax,0
call printf
mov rdi,1
call exit
ret

; void list_set(void * list,int index, Handler * value)
; rdi = list
; rsi = index
; rdx = value
_list_set:
%if !FAST
  push rax
  list_size rdi
  cmp rsi,rax
  jl .proceed
  mov rdi,rax
  call list_index_out_bound
  .proceed:
%endif

  mov [rdi+24+(rsi*8)],rdx

%if !FAST
  pop rax
%endif
ret

msg_index: db "handlers[ %d ]",10,0
; Handler * _list_find_h_by_type (void*list,int type)
_list_find_h_by_type:
  ; edi list
  ; esi type

  multipush rbx,rcx,rdx
  mov rbx,0              ; counter

  list_size rdi          ; edi list
  mov rcx,rax            ; list size
  sub rcx,1
.loop:
  %if DEBUG
    multipush rdi,rsi,rax,rcx
    mov rdi,msg_index
    mov rsi,rbx
    mov rax,0
    call printf
    multipop rdi,rsi,rax,rcx
  %endif

  cmp  rbx,rcx
  jle .logic
  jmp .no_found
  ; logic
.logic:
  list_get rdi,rbx
  mov rdx,rax
  cmp rax, 0
  je ._inc
  cmp rsi,H_TYPE(rdx)
  je .set_rax
._inc:
  inc  rbx
  jmp .loop
  ; logic
.set_rax:
  mov rax,rdx
  jmp .end
.no_found:
  mov rdi, msg_no_found
  mov rax,0
  call printf
  call _main_handler_
  mov rdi,1
  call exit
.end:
  multipop rbx,rcx,rdx
  ret
  ; _list_find_h_by_type

h_malloc_error:
    mov rdi, msg_malloc_error
    mov rax,0
    call printf
    ret

_top:                    ; obtiene el tope de la pila, sin desapilar
    mov rax,[exception_stack+(rcx*8)]
    ret

_print_top:              ; imprime
    push rcx
    call _top

    mov rdi, msg_dump_stack_item
    mov rsi,rax
    mov rax,0
    call printf
    pop rcx
    ret

_dump_stack:             ; mostrar valores de la pila
   push rax
   push rcx
   mov rdi, msg_dump_stack_head
   mov rax,0
   call printf
   mov rcx,0

   .loop:
   cmp  rcx,[exception_count_stack]
   je .end

   ; push  rcx             ; apilo rcx, al llamar _print_top, se modifica
   call _print_top

   ; pop  rcx              ; desapilo por modificacion anterior
   inc  rcx
   jmp .loop             ; vuelvo arriba para imprimir el siguiente

   .end:
   mov rdi, msg_dump_stack_foot
   mov rax,0
   call printf
   pop rcx
   pop rax
   ret

   ; Manejador en tope de pila, manejador principal
   ; este manejador se usara cuando para mostrar
   ; un mensaje de excepcion por defecto
   ; o para cuando la pila quede sin arreglo de
   ; manejadores, y sale del programa
   ; TODO mejorar la informacion q muestra
_main_handler_:
    ;push rbp
    ;mov rbp,rsp
    ;sub rsp,8

    mov rdi,_msg_main_handler_
    mov rax,0
    call printf

    mov rdi,1
    call exit
_msg_main_handler_: db "Manejador principal. Saliendo del programa. exit(1)",10,0


%endif
