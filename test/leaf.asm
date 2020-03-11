


%include "feactures/exceptions/linux64.asm"
;global
	global main 
;extern
	extern printf 
	extern puts 
section .data
section .bss
section .text
main:
	push rbp
	mov rbp,rsp
	sub rsp,24
	mov rax,1
	mov [rbp-8],rax
	;start try  _BZJ0y1Ke_
	push  rax
	new_list 1
	mov rbx,rax
	new_handler 1,_PS35pfpC_
	list_set rbx,0,rax
	_push_ rbx
	pop  rax
	;end prepare try  _BZJ0y1Ke_
	;start try  _YkQlRjhz_
	push  rax
	new_list 1
	mov rbx,rax
	new_handler 1,_tM8eBH7Q_
	list_set rbx,0,rax
	_push_ rbx
	pop  rax
	;end prepare try  _YkQlRjhz_
	mov rax,10
	mov [rbp-8],rax
	;throw 
	_pop_ 
	mov rcx,rax
	list_find_h_by_type rax,1
	mov rbx,rax
	mov rax,20
	mov rax,rax
	mov rbp,[rcx+8]
	mov rsp,[rcx+16]
	J_H_RUTINE rbx
	;end throw 
	jmp _YkQlRjhz_
	;start catch 
_tM8eBH7Q_:
	mov [rbp-16],rax
	mov rax,[rbp-16]
	mov [rbp-8],rax
	;throw 
	_pop_ 
	mov rcx,rax
	list_find_h_by_type rax,1
	mov rbx,rax
	mov rax,[rbp-8]
	add rax,5
	mov rax,rax
	mov rbp,[rcx+8]
	mov rsp,[rcx+16]
	J_H_RUTINE rbx
	;end throw 
	;end catch 
	jmp _saaNBTFA_
	;end try  _YkQlRjhz_
_YkQlRjhz_:
	_pop_ 
_saaNBTFA_:
	jmp _BZJ0y1Ke_
	;start catch 
_PS35pfpC_:
	mov [rbp-24],rax
	mov rax,[rbp-24]
	mov [rbp-8],rax
	;end catch 
	jmp _PKXopZpx_
	;end try  _BZJ0y1Ke_
_BZJ0y1Ke_:
	_pop_ 
_PKXopZpx_:
	mov rax,[rbp-8]
_C6mTcIIJ_:
	mov rsp,rbp
	pop rbp
	ret 
