main:
push rbp
mov rbp,rsp
sub rsp,0
mov rax,[rbx+24]
add rax,[rbx+8]
mov rbx,[rbx+16]
add rbx,[rbx+8]
add rax,rbx
mov rbx,[rbx+8]
add rbx,[rbx+16]
add rax,rbx
mov [rbx+16],rax
mov rsp,rbp
pop rbp
ret 
