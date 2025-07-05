	.section	.note.GNU-stack,"",@progbits
	.section .data
	.globl _temp0
	.align 4
_temp0:
	.space 4
	.globl _temp1
	.align 4
_temp1:
	.space 1
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
	.space 1
	.globl aux
	.align 4
aux:
	.long 2
	.globl quantidade
	.align 4
quantidade:
	.space 4
	.globl valor
	.align 4
valor:
	.space 4
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
	.globl LIT0
	.align 4
LIT0:
	.long 10
	.globl LIT1
	.align 4
LIT1:
	.long 0
	.globl LIT2
	.align 4
LIT2:
	.long 2
	.globl LIT3
	.align 4
LIT3:
	.long 5
	.globl LIT4
	.align 4
LIT4:
	.long 12
	.globl LIT5
LIT5:
	.string "valor da variavel quantidade antes do while loop: "
	.globl LIT6
LIT6:
	.string "\n"
	.globl LIT7
	.align 4
LIT7:
	.long 50
	.globl LIT8
LIT8:
	.string "valor atingido: "
	.globl LIT9
LIT9:
	.string "valor maior que 50\n"
	.globl LIT10
LIT10:
	.string "valor da variavel quantidade no loop: "
	.globl LIT11
	.align 4
LIT11:
	.long 1
	.globl LIT12
	.align 4
LIT12:
	.long 15
	.globl LIT13
LIT13:
	.string "valor da variavel quantidade depois do loop: "
	.section .text
	.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	# Atribuição INT
	movl	LIT3(%rip), %eax
	movl	%eax, x(%rip)
	# Preparando argumentos para a chamada de multiplica
	movl	LIT4(%rip), %edi
	movl	LIT2(%rip), %esi
	call	multiplica
	# Armazenando valor de retorno de multiplica em _temp0
	movl	%eax, _temp0(%rip)
	# Atribuição INT
	movl	_temp0(%rip), %eax
	movl	%eax, y(%rip)
	# Imprimindo valor: y
	movl	y(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leave
	ret
multiplica:
	pushq %rbp
	movq %rsp, %rbp
	# Movendo argumento 1 (quantidade) do registrador para a variavel global
	movl	%edi, quantidade(%rip)
	# Movendo argumento 2 (valor) do registrador para a variavel global
	movl	%esi, valor(%rip)
	# Imprimindo valor: "valor da variavel quantidade antes do while loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT5(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT6(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
_label2:
	movl	valor(%rip), %eax
	movl	LIT7(%rip), %edx
	cmpl	%edx, %eax
	setg	%al
	movb	%al, _temp1(%rip)
	# Salto condicional para _label0
	movzbl	_temp1(%rip), %eax
	testl	%eax, %eax
	je	_label0
	movl	valor(%rip), %eax
	imull	aux(%rip), %eax
	movl	%eax, _temp2(%rip)
	# Atribuição INT
	movl	_temp2(%rip), %eax
	movl	%eax, valor(%rip)
	# Imprimindo valor: "valor atingido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT8(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: valor
	movl	valor(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT6(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor maior que 50\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT9(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Salto incondicional para _label1
	jmp	_label1
_label0:
	movl	valor(%rip), %eax
	imull	aux(%rip), %eax
	movl	%eax, _temp3(%rip)
	# Atribuição INT
	movl	_temp3(%rip), %eax
	movl	%eax, valor(%rip)
_label1:
	# Imprimindo valor: "valor da variavel quantidade no loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT10(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT6(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	movl	quantidade(%rip), %eax
	movl	LIT11(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp4(%rip)
	# Atribuição INT
	movl	_temp4(%rip), %eax
	movl	%eax, quantidade(%rip)
	movl	quantidade(%rip), %eax
	movl	LIT12(%rip), %edx
	cmpl	%edx, %eax
	setl	%al
	movb	%al, _temp5(%rip)
	# Salto condicional para _label2
	movzbl	_temp5(%rip), %eax
	testl	%eax, %eax
	jne	_label2
	# Imprimindo valor: "valor da variavel quantidade depois do loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT13(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT6(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor atingido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT8(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: valor
	movl	valor(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT6(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Retornando INT
	movl	valor(%rip), %eax
	leave
	ret
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
