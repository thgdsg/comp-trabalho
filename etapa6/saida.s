	.section	.note.GNU-stack,"",@progbits
	.section .data
	.globl _temp0
	.align 4
_temp0:
	.space 4
	.globl _temp1
	.align 4
_temp1:
	.space 4
	.globl _temp2
	.align 4
_temp2:
	.space 4
	.globl _temp3
	.align 4
_temp3:
	.space 4
	.globl _temp4
	.align 4
_temp4:
	.space 4
	.globl _temp5
	.align 4
_temp5:
	.space 4
	.globl contagem
	.align 4
contagem:
	.space 4
	.globl entrada
	.align 4
entrada:
	.long 0
	.globl quantidade
	.align 4
quantidade:
	.space 4
	.globl retorno
	.align 4
retorno:
	.long 0
	.globl x
	.align 4
x:
	.long 10
	.globl y
	.align 4
y:
	.long 0

	# Literais e Formatos de Impressao/Leitura
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
.LIT0:
	.align 4
	.long 10
.LIT1:
	.align 4
	.long 0
.LIT2:
	.string "contagem maior que 10\n"
.LIT3:
	.string "contagem menor que 10\n"
.LIT4:
	.align 4
	.long 1
.LIT5:
	.align 4
	.long 2
	.section .text
	.globl main
multiplica:
	pushq %rbp
	movq %rsp, %rbp
	# Movendo argumento 1 (quantidade) do registrador para a variavel global
	movl	%edi, quantidade(%rip)
	# Movendo argumento 2 (contagem) do registrador para a variavel global
	movl	%esi, contagem(%rip)
_label2:
	movl	contagem(%rip), %eax
	cmpl	.LIT0(%rip), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, _temp0(%rip)
	# Salto condicional para _label0
	cmpl	$0, _temp0(%rip)
	je	_label0
	# Imprimindo valor: "contagem maior que 10\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	.LIT2(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	movl	contagem(%rip), %eax
	imull	contagem(%rip), %eax
	movl	%eax, _temp1(%rip)
	# Atribuição INT
	movl	_temp1(%rip), %eax
	movl	%eax, contagem(%rip)
	# Salto incondicional para _label1
	jmp	_label1
_label0:
	# Imprimindo valor: "contagem menor que 10\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	.LIT3(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	movl	contagem(%rip), %eax
	imull	contagem(%rip), %eax
	movl	%eax, _temp2(%rip)
	# Atribuição INT
	movl	_temp2(%rip), %eax
	movl	%eax, contagem(%rip)
_label1:
	movl	quantidade(%rip), %eax
	subl	.LIT4(%rip), %eax
	movl	%eax, _temp3(%rip)
	# Atribuição INT
	movl	_temp3(%rip), %eax
	movl	%eax, quantidade(%rip)
	movl	quantidade(%rip), %eax
	cmpl	.LIT1(%rip), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, _temp4(%rip)
	# Salto condicional para _label2
	cmpl	$0, _temp4(%rip)
	jne	_label2
	# Retornando INT
	movl	contagem(%rip), %eax
	leave
	ret
	leave
	ret
main:
	pushq %rbp
	movq %rsp, %rbp
	# Preparando argumentos para a chamada de multiplica
	movl	.LIT5(%rip), %edi
	movl	.LIT5(%rip), %esi
	call	multiplica
	# Armazenando valor de retorno de multiplica em _temp5
	movl	%eax, _temp5(%rip)
	# Atribuição INT
	movl	_temp5(%rip), %eax
	movl	%eax, y(%rip)
	# Imprimindo valor: y
	movl	y(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leave
	ret

# --- Funcoes Auxiliares ---
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
