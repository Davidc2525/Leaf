;global
;extern
section .data
section .bss
section .text
main:
push rbp
mov rbp,rsp
sub rsp,32
mov rax,[rbp+8]
add rax,[rbp+16]
mov rbx,[rbp+24]
add rbx,[rbp+32]
add rax,rbx
mov [rbp+24],rax
mov rsp,rbp
pop rbp
ret 
