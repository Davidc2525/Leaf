;global
	global main 
;extern
	extern printf 
section .data
section .bss
section .text
main:
	push rbp
	mov rbp,rsp
	sub rsp,8
	mov rax,25
	mov [rbp-8],rax
	mov rsp,rbp
	pop rbp
	ret 
