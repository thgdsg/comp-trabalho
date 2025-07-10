	.section	.note.GNU-stack,"",@progbits
	.section .data
	.globl _temp0
	.align 4
_temp0:
	.space 8
	.globl _temp1
	.align 4
_temp1:
	.space 8
	.globl _temp2
	.align 4
_temp2:
	.space 8
	.globl _temp3
	.align 4
_temp3:
	.space 8
	.globl _temp4
	.align 4
_temp4:
	.space 8
	.globl _temp5
	.align 4
_temp5:
	.space 8
	.globl _temp6
	.align 4
_temp6:
	.space 8
	.globl _temp7
	.align 4
_temp7:
	.space 8
	.globl _temp8
	.align 4
_temp8:
	.space 8
	.globl naosei
	.align 4
naosei:
	.long 5
	.long 2
	.globl qqr_coisa
	.align 4
qqr_coisa:
	.space 8
	.globl retorno
	.align 4
retorno:
	.space 8
	.globl x
	.align 4
x:
	.long 10
	.globl y
	.align 4
y:
	.byte 255
	.globl z
	.align 4
z:
	.byte 1
	.byte 2
	.byte 3
	.byte 4
	.byte 5
	.globl numeros
	.align 4
numeros:
	.long 1
	.long 2
	.long 2
	.long 3
	.long 3
	.long 4
	.globl pequeno
	.align 4
pequeno:
	.space 8

	# Formatos de Impressao/Leitura
.LC_INT:
	.string "%d "
.LC_REAL:
	.string "%d/%d "
.LC_CHAR:
	.string "%c "
.LC_STRING:
	.string "%s"
.LC_SCAN_INT:
	.string "%d"
.LC_SCAN_REAL:
	.string "%d/%d"
.LC_SCAN_STRING:
	.string "%255s"
.LC_SCAN_CHAR:
	.string " %c"
	.comm .read_buffer, 256, 32
	.globl LIT0
	.align 4
LIT0:
	.long 10
	.globl LIT1
	.align 4
LIT1:
	.long 5
	.long 2
	.globl LIT2
	.align 4
LIT2:
	.long 255
	.globl LIT3
	.align 4
LIT3:
	.long 5
	.globl LIT4
	.align 4
LIT4:
	.long 1
	.globl LIT5
	.align 4
LIT5:
	.long 2
	.globl LIT6
	.align 4
LIT6:
	.long 3
	.globl LIT7
	.align 4
LIT7:
	.long 4
	.globl LIT8
	.align 4
LIT8:
	.long 1
	.long 2
	.globl LIT9
	.align 4
LIT9:
	.long 2
	.long 3
	.globl LIT10
	.align 4
LIT10:
	.long 3
	.long 4
	.globl LIT11
	.align 4
LIT11:
	.long 5
	.long 10
	.globl LIT12
	.align 4
LIT12:
	.long 0
	.globl LIT13
	.align 4
LIT13:
	.long 01
	.long 5
	.globl LIT14
LIT14:
	.string "antes da funcao: "
	.globl LIT15
LIT15:
	.string "\n"
	.globl LIT16
LIT16:
	.string "dps da funcao: "
	.section .text
	.globl main
funcaozinha:
	pushq %rbp
	movq %rsp, %rbp
	# Movendo argumento 1 (retorno) do registrador pra variavel global
	movl	%edi, retorno(%rip)
	movl	%esi, 4+retorno(%rip)
	# Movendo argumento 2 (qqr_coisa) do registrador pra variavel global
	movl	%edx, qqr_coisa(%rip)
	movl	%ecx, 4+qqr_coisa(%rip)
	# Lendo elemento de vetor: numeros[1]
	movl	LIT4(%rip), %ecx
	leaq	numeros(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp0(%rip)
	# Atribuindo a elemento de vetor: numeros[1] = 5/10
	movl	LIT4(%rip), %ecx
	leaq	numeros(%rip), %rax
	movl	LIT11(%rip), %edx
	movl	4+LIT11(%rip), %r8d
	movl	%edx, (%rax, %rcx, 8)
	movl	%r8d, 4(%rax, %rcx, 8)
	# Real ADD
	movl	retorno(%rip), %r8d
	movl	4+retorno(%rip), %r9d
	movl	qqr_coisa(%rip), %r10d
	movl	4+qqr_coisa(%rip), %r11d
	movslq	%r8d, %rax
	imulq	%r11, %rax
	movslq	%r10d, %rbx
	imulq	%r9, %rbx
	addq	%rbx, %rax
	movslq	%r9d, %rcx
	imulq	%r11, %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	call	_simplify_fraction
	movl	%eax, _temp1(%rip)
	movl	%edx, 4+_temp1(%rip)
	# Atribuição REAL
	movl	_temp1(%rip), %eax
	movl	4+_temp1(%rip), %edx
	movl	%eax, retorno(%rip)
	movl	%edx, 4+retorno(%rip)
	# Retornando REAL
	movl	retorno(%rip), %eax
	movl	4+retorno(%rip), %edx
	# Saindo da função por meio do return
	leave
	ret
	# Fim da função funcaozinha
	leave
	ret
main:
	pushq %rbp
	movq %rsp, %rbp
	# Lendo elemento de vetor: pequeno[0]
	movl	LIT12(%rip), %ecx
	leaq	pequeno(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp2(%rip)
	# Atribuindo a elemento de vetor: pequeno[0] = 01/5
	movl	LIT12(%rip), %ecx
	leaq	pequeno(%rip), %rax
	movl	LIT13(%rip), %edx
	movl	4+LIT13(%rip), %r8d
	movl	%edx, (%rax, %rcx, 8)
	movl	%r8d, 4(%rax, %rcx, 8)
	# Lendo elemento de vetor: pequeno[0]
	movl	LIT12(%rip), %ecx
	leaq	pequeno(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp3(%rip)
	# Imprimindo valor: "antes da funcao: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: _temp3
	movl	_temp3(%rip), %esi
	movl	4+_temp3(%rip), %edx
	leaq	.LC_REAL(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo elemento de vetor: pequeno[0]
	movl	LIT12(%rip), %ecx
	leaq	pequeno(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp4(%rip)
	# Lendo elemento de vetor: numeros[2]
	movl	LIT5(%rip), %ecx
	leaq	numeros(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp5(%rip)
	# Lendo elemento de vetor: numeros[1]
	movl	LIT4(%rip), %ecx
	leaq	numeros(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp6(%rip)
	# Real MUL
	movl	_temp5(%rip), %r8d
	movl	4+_temp5(%rip), %r9d
	movl	_temp6(%rip), %r10d
	movl	4+_temp6(%rip), %r11d
	movslq	%r8d, %rax
	imulq	%r10, %rax
	movslq	%r9d, %rcx
	imulq	%r11, %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	call	_simplify_fraction
	movl	%eax, _temp7(%rip)
	movl	%edx, 4+_temp7(%rip)
	# Atribuindo a elemento de vetor: pequeno[0] = _temp7
	movl	LIT12(%rip), %ecx
	leaq	pequeno(%rip), %rax
	movl	_temp7(%rip), %edx
	movl	4+_temp7(%rip), %r8d
	movl	%edx, (%rax, %rcx, 8)
	movl	%r8d, 4(%rax, %rcx, 8)
	# Lendo elemento de vetor: pequeno[0]
	movl	LIT12(%rip), %ecx
	leaq	pequeno(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp8(%rip)
	# Imprimindo valor: "dps da funcao: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT16(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: _temp8
	movl	_temp8(%rip), %esi
	movl	4+_temp8(%rip), %edx
	leaq	.LC_REAL(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Fim da função main
	leave
	ret

# --- Funcoes Auxiliares ---
_read_and_convert:
	pushq	%rbp
	movq	%rsp, %rbp
	leaq	.LC_SCAN_STRING(%rip), %rdi
	leaq	.read_buffer(%rip), %rsi
	movl	$0, %eax
	call	scanf@PLT
	leaq	.read_buffer(%rip), %rdi
	call	strlen@PLT
	cmpq	$1, %rax
	jne	_convert_atoi
	movzbl	.read_buffer(%rip), %eax
	cmpb	$48, %al
	jl	_is_char
	cmpb	$57, %al
	jle	_convert_atoi
_is_char:
	movzbl	.read_buffer(%rip), %eax
	jmp	_read_convert_end
_convert_atoi:
	leaq	.read_buffer(%rip), %rdi
	call	atoi@PLT
_read_convert_end:
	leave
	ret

_gcd:
	movq	%rdi, %rax
	movq	%rsi, %rbx
_gcd_loop:
	cmpq	$0, %rbx
	je	_gcd_end
	movq	%rbx, %rcx
	movq	%rax, %rdx
	movq	$0, %rax
	movq	%rdx, %rax
	movq	$0, %rdx
	idivq	%rcx
	movq	%rcx, %rax
	movq	%rdx, %rbx
	jmp	_gcd_loop
_gcd_end:
	ret
_simplify_fraction:
	pushq	%rdi
	pushq	%rsi
	call	_gcd
	popq	%rsi
	popq	%rdi
	movq	%rax, %rbx
	movq	%rdi, %rax
	movq	$0, %rdx
	idivq	%rbx
	pushq	%rax
	movq	%rsi, %rax
	movq	$0, %rdx
	idivq	%rbx
	movq	%rax, %rdx
	popq	%rax
	ret
