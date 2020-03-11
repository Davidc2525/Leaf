


; Manejador de excepciones  para linux 32
; autor: Colmenares David, david25pcxtreme@gmail.com

%ifndef _H_
%define _H_
; activa debug, para comprobacion en desarrollo
%define DEBUG 0
; evita la expacion de codigo de comprobacion en algunos casos
%define FAST 0
; tamaño maximo de la pila de excepciones
%define EXCEPTION_MAX 1000
extern printf
extern _malloc
extern calloc
extern free
extern exit
; global _list_find_h_by_type

; list to save handles-----
; void * new_list(int size)
%macro new_list 1
    push 4
    push dword 4+(%1)
    call calloc
    add esp, 8
    mov dword [eax],%1   ; store size by self
    test eax,eax
    je h_malloc_error
%endmacro

; void list_set(void * list,int index, Handler * value)
%macro list_set 3
  mov edi,%1
  mov esi,%2
  mov edx,%3
  call _list_set
  ; mov dword [%1+4+(%2*4)],%3
%endmacro

; %define list_set(list,index,value) mov dword [list+(index*4)],value

; void * list_get(void * list,int index)
%macro list_get 2
    mov dword eax, [%1+4+(%2*4)]
%endmacro

; int list_size(void * list)
%macro list_size 1
   mov dword eax, [%1]
%endmacro

; Handler * list_find_h_by_type (void*list,int type)
%macro list_find_h_by_type 2
    mov edi,%1
    mov esi,%2
    call _list_find_h_by_type
%endmacro


; --------------------
; class Handler {
; int type;
; long rutine;
; }
%define H_SIZE     12    ; no property
%define H_TYPE(o)  [o]   ; 4 bytes int
%define H_RUTINE(o)[o+4] ; 8 bytes long
%macro J_H_RUTINE 1
    jmp H_RUTINE(%1)
%endmacro

%macro new_handler 0
    push 4
    push H_SIZE
    call calloc
    add esp, 8
    test eax,eax
    je h_malloc_error
%endmacro

%macro new_handler 2
    new_handler
    mov dword H_TYPE(eax),%1
    mov dword H_RUTINE(eax),%2
%endmacro

%macro delete_handler 1
    push %1
    call free
    add esp, 4
%endmacro

; -------------------

%macro _push_ 1
    mov dword edi,%1
    call _push_map_exception
%endmacro

%macro _pop_ 0
    call _pop_map_exception
%endmacro


section .bss
    exception_stack resd EXCEPTION_MAX
    ; inst1 resb 12
section .data

    exception_count_stack dd 0
   ; msg db '%d',10, 0
    msg_dump_stack_head db "stack -> [",0
    msg_dump_stack_item db " %d ",0
    msg_dump_stack_foot db "]",10,0
    msg_malloc_error db "error al reservar memoria",10,0
    msg_no_found db "ERROR: no se encotro este manejador (bloque catch) para el tipo: %d.",10,0

section .text
_push_map_exception:
   ; _push:
   push ebx
   mov ebx,   [exception_count_stack]
   mov dword  [exception_stack+(ebx*4)],edi
   inc dword  [exception_count_stack]
   pop ebx
   ret
_pop_map_exception:
    ; _pop:
    push ebx
    dec dword [exception_count_stack]
    mov ebx,  [exception_count_stack]
    mov eax,  [exception_stack+(ebx*4)]
    mov dword [exception_stack+(ebx*4)],0
    pop ebx
    ret

    ; helpers
msg_out_bound: db "Indice mayor al tamaño de la lista: index(%d) -> list_size(%d)",10,0
list_index_out_bound:
push dword edi
push dword esi
push msg_out_bound
call printf
add esp,4
call exit
ret

; void list_set(void * list,int index, Handler * value)
; edi = list
; esi = index
; edx = value
_list_set:
%if !FAST
  push eax
  list_size edi
  cmp dword esi,eax
  jl .proceed
  mov edi,eax
  call list_index_out_bound
  .proceed:
%endif

  mov dword [edi+4+(esi*4)],edx

%if !FAST
  pop eax
%endif
ret

msg_index: db "handles[ %d ]",10,0
; Handler * _list_find_h_by_type (void*list,int type)
_list_find_h_by_type:
  ; edi list
  ; esi type
  push ebx
  push ecx
  push edx

  mov ebx,0              ; counter

  list_size edi          ; edi list
  mov ecx,eax            ; list size
  sub ecx,1
.loop:
  %if DEBUG
    push dword ebx
    push msg_index
    call printf
    add esp,8
  %endif

  cmp dword ebx,ecx
  jle .logic
  jmp .no_found
  ; logic
.logic:
  list_get edi,ebx
  mov edx,eax
  cmp dword eax,0
  je ._inc
  cmp dword esi,H_TYPE(edx)
  je .set_eax
._inc:
  inc dword ebx
  jmp .loop
  ; logic
.set_eax:
  mov eax,edx
  jmp .end
.no_found:
  push dword esi
  push msg_no_found
  call printf
  add esp,4
  push 1
  call exit
  add esp,4
.end:
  pop edx
  pop ecx
  pop ebx
ret
; _list_find_h_by_type

h_malloc_error:
    push msg_malloc_error
    call printf
    add esp,4
    ret

_top:                    ; obtiene el tope de la pila, sin desapilar
    mov eax,[exception_stack+(ecx*4)]
    ret

_print_top:              ; imprime
    call _top
    push eax
    push msg_dump_stack_item
    call printf
    add esp,8
    ret

_dump_stack:             ; mostrar valores de la pila
   push msg_dump_stack_head
   call printf
   add esp,4

   mov ecx,0

   .loop:
   cmp dword ecx,[exception_count_stack]
   je .end

   push dword ecx        ; apilo ecx, al llamar _print_top, se modifica
   call _print_top

   pop dword ecx         ; desapilo por modificacion anterior
   inc dword ecx
   jmp .loop             ; vuelvo arriba para imprimir el siguiente

   .end:
   push msg_dump_stack_foot
   call printf
   add esp,4
   ret



%endif