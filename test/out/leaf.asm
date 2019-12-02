; iniciando tabla de simbolos
; insert st, obj kind: 2, name bool, type kind: 8, level: 0
; insert st, obj kind: 2, name byte, type kind: 13, level: 0
; insert st, obj kind: 2, name int, type kind: 1, level: 0
; insert st, obj kind: 2, name float, type kind: 2, level: 0
; insert st, obj kind: 2, name double, type kind: 3, level: 0
; insert st, obj kind: 2, name string, type kind: 5, level: 0
;  - elemtype kind: 11
; insert st, obj kind: 2, name char, type kind: 11, level: 0
; insert st, obj kind: 0, name null, type kind: 6, level: 0
; insert st, obj kind: 2, name void, type kind: 0, level: 0
; fun main retorna ; insert st, obj kind: 4, name main, type kind: 0, level: 0
; find st, obj kind: 2, name int, type kind: 1
;int
 ;kind: 9
  ;kind: 1
;nPars 0
;nPars 0

; flo es float

; find st, obj kind: 2, name float, type kind: 2
; insert st, obj kind: 1, name flo, type kind: 2, level: 1
;current adr 8 nvars 0
;adr: 8
;kind: 2
;StatementBlockAdd 0x246c030 0x246e710
;FuncDeclStm::add 0x246e710
; find st, obj kind: 1, name flo, type kind: 2
; load flo
; insert st, obj kind: 1, name , type kind: 2, level: 1
;current adr 16 nvars 1
; insert st, obj kind: 1, name , type kind: 2, level: 1
;current adr 24 nvars 2
;  declared twice
; insert st, obj kind: 1, name , type kind: 2, level: 1
;current adr 32 nvars 3
;  declared twice
;  declared twice
;StatementBlockAdd 0x246c030 0x246f5a0
;FuncDeclStm::add 0x246f5a0
;StatementBlockAdd 0x246c030 0x246fa50
;FuncDeclStm::add 0x246fa50
;StatementBlockAdd 0x2469c90 0x246c030
;-----------------------------
section .data
; strings literals (0)
formato db "%d",10,0
formato_real db "%f",10,0
formato_string db "%s",0
formato_char db "%c",10,0

section .text
global main
extern printf
; simbolos externos (0)

main:
PUSH rbp
MOV rbp, rsp
SUB rsp,32

;ASSIGNMENT from binop
mov DWORD [rbp-16],__float32__(2.590000) ;const float,  loadplace: 3
fld DWORD [rbp-16]
mov DWORD [rbp-24],__float32__(2.500000) ;const float,  loadplace: 3
fld DWORD [rbp-24]
fadd; ;const floa
mov DWORD [rbp-32],__float32__(20.000000) ;const float,  loadplace: 3
fld DWORD [rbp-32]
fadd; ;const floa
FSTP DWORD [rbp-8] ;store place: 3
movss xmm0,[rbp-8]
cvtss2sd xmm0,xmm0

mov rdi , formato_real
mov rax,1
call printf
MOV rax,0 ;const
mov rsp,rbp 
pop rbp 
ret  

MOV rsp,rbp 
POP rbp 
RET


