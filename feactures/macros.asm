


%ifndef __MACROS__
%define __MACROS__ 

%if __BITS__ == 32

%macro _enter32 0
    push ebp
    mov ebp,esp
%endmacro

%macro _enter32 1
    push ebp
    mov ebp,esp
    sub esp,%1
%endmacro

%macro _exit32 0
    mov esp,ebp
    pop ebp
    ret
%endmacro

%endif

%if __BITS__ == 64
; macros 64
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

%endif

%macro  string 2 

    %1: db      %2

%endmacro

%macro  stringln 2 

    %1: db      %2,10,0

%endmacro



%endif
