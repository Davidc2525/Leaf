

;nasm -f elf64  feactures/polymorphis/textLinux64.asm  && gcc feactures/polymorphis/textLinux64.o -o feactures/polymorphis/a.out && feactures/polymorphis/a.out 
global main

extern printf
extern puts
extern calloc
extern malloc


%macro _enter64 0
    push rbp
    mov rbp,rsp
%endmacro

%macro _enter64 1
    push rbp
    mov rbp,rsp
    sub rsp,%1
%endmacro

%macro _exit64 0
    mov rsp,rbp
    pop rbp
    ret
%endmacro

%macro new_vtable 1
    mov rdi,(%1)
    ;mov rsi,8
    call malloc
%endmacro

%macro new_obj 2
    push rbx
    mov rdi,8+(%1)
    ;mov rsi,8
    call malloc
    mov rbx,rax
    new_vtable %2
    mov [rbx],rax
    mov rax,rbx
    pop rbx
%endmacro

;void set_in_vtable(void * obj,int index,void * meth)
%macro set_in_vtable 3
    mov rax,%1
    mov rax,[rax]
    mov qword [rax+(%2*8)],%3
%endmacro

;void*meth get_in_vtable(void * obj,int index)
%macro get_in_vtable 2
    mov rax,%1
    mov rax,[rax]
    mov rax,[rax+(%2*8)]
%endmacro


msg: db "obj %ld",10,0
main: 
    _enter64 16

    new_obj 8,(8*1)
    mov [rbp-8],rax  
    set_in_vtable [rbp-8],0,class1_meht
    get_in_vtable [rbp-8],0
    mov rdi,[rbp-8]
    mov qword [rdi+8],25
    call rax  

    new_obj 8,(8*1)
    mov [rbp-16],rax
    set_in_vtable [rbp-16],0,class2_meht
    get_in_vtable [rbp-16],0
    mov rdi,[rbp-16]
    mov qword [rdi+8],26
    call rax
    
    mov rsi,rax
    mov rdi,msg
    mov rax,0
    call printf
    _exit64

msg2: db "class_1 meth: %d",10,0
class1_meht:
_enter64 8
    mov [rbp-8],rdi;obj
    mov rsi,[rdi+8]
    mov rdi,msg2
    mov rax,0
    call printf
_exit64

msg3: db "class_2 meth: %d",10,0
class2_meht:
_enter64 8
    mov [rbp-8],rdi;obj
    mov rsi,[rdi+8]
    mov rdi,msg3
    mov rax,0
    call printf
_exit64