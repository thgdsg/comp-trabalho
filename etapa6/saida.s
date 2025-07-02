	.section .data
	.comm _temp0,4,4
	.comm _temp1,4,4
	.comm _temp10,4,4
	.comm _temp11,4,4
	.comm _temp12,4,4
	.comm _temp13,4,4
	.comm _temp14,4,4
	.comm _temp15,4,4
	.comm _temp16,4,4
	.comm _temp17,4,4
	.comm _temp18,4,4
	.comm _temp19,4,4
	.comm _temp2,4,4
	.comm _temp20,4,4
	.comm _temp21,4,4
	.comm _temp22,4,4
	.comm _temp23,4,4
	.comm _temp3,4,4
	.comm _temp4,4,4
	.comm _temp5,4,4
	.comm _temp6,4,4
	.comm _temp9,4,4
	.globl a
a:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl alface
alface:
	.align 1
	.byte 0
	.globl b
b:
	.align 1
	.byte 0
	.globl batata
batata:
	.align 4
	.long 0
	.long 0
	.globl h
h:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl j
j:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl n
n:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl number
number:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl p
p:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl tomate
tomate:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl x
x:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl y
y:
	.align 4
	.long 0
	.align 4
	.long 0
	.long 0
	.globl z
	.align 4
z:
	.long 0
	.long 0
	.long 1
	.globl matrix
	.align 4
matrix:
	.space 1320

	# Literais
.LIT19:
	.string "."
.LIT14:
	.string "banana"
.LIT15:
	.string "batata"
.LIT20:
	.string "qwaerfwearf"
.LIT6:
	.align 4
	.long 65
.LIT3:
	.align 4
	.long 74
.LIT22:
	.align 4
	.long 97
.LIT4:
	.align 4
	.long 98
.LIT12:
	.align 4
	.long 105
.LIT13:
	.align 4
	.long 108
.LIT8:
	.align 4
	.long 0
.LIT11:
	.align 4
	.long 01
.LIT17:
	.align 4
	.long 01
	.long 2
.LIT0:
	.align 4
	.long 1
.LIT16:
	.align 4
	.long 10
.LIT1:
	.align 4
	.long 2
.LIT18:
	.align 4
	.long 20
.LIT5:
	.align 4
	.long 21
.LIT2:
	.align 4
	.long 3
.LIT7:
	.align 4
	.long 330
.LIT10:
	.align 4
	.long 5
.LIT21:
	.align 4
	.long 5
	.long 3
.LIT9:
	.align 4
	.long 9
	.section .text
	.globl main
func:
	pushq %rbp
	movq %rsp, %rbp
	cmpl	$0, _temp0(%rip)
	je	_label0
	movl	h(%rip), %eax
	movl	j(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp1(%rip)
	cmpl	$0, 0
	jne	_label1
_label0:
_label1:
	movl	.LIT9(%rip), %eax
	movl	_temp3(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp4(%rip)
	movl	_temp4(%rip), %eax
	movl	%eax, x(%rip)
	movl	.LIT11(%rip), %eax
	movl	%eax, a(%rip)
	movl	z(%rip), %eax
	movl	%eax, b(%rip)
	movl	a(%rip), %eax
	movl	b(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp5(%rip)
	movl	_temp5(%rip), %eax
	movl	x(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp6(%rip)
	leave
	ret
vegetais:
	pushq %rbp
	movq %rsp, %rbp
	leave
	ret
main:
	pushq %rbp
	movq %rsp, %rbp
	movl	_temp7(%rip), %eax
	movl	%eax, y(%rip)
_label2:
	movl	.LIT0(%rip), %eax
	movl	%eax, x(%rip)
	movl	.LIT1(%rip), %eax
	movl	%eax, y(%rip)
	movl	.LIT2(%rip), %eax
	movl	%eax, a(%rip)
	movl	.LIT10(%rip), %eax
	movl	%eax, b(%rip)
	movl	.LIT1(%rip), %eax
	movl	_temp10(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp11(%rip)
	cmpl	$0, _temp13(%rip)
	jne	_label2
_label3:
	cmpl	$0, _temp14(%rip)
	je	_label4
	movl	x(%rip), %eax
	movl	b(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp15(%rip)
	movl	_temp15(%rip), %eax
	movl	%eax, x(%rip)
	cmpl	$0, _temp14(%rip)
	jne	_label3
_label4:
	movl	_temp17(%rip), %eax
	movl	.LIT2(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp18(%rip)
	movl	_temp18(%rip), %eax
	movl	%eax, a(%rip)
	cmpl	$0, _temp22(%rip)
	je	_label5
	movl	.LIT1(%rip), %eax
	movl	%eax, y(%rip)
_label5:
	leave
	ret
incm:
	pushq %rbp
	movq %rsp, %rbp
	cmpl	$0, _temp23(%rip)
	je	_label6
_label6:
	leave
	ret
