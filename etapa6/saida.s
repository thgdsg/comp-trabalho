	.section	.note.GNU-stack,"",@progbits
	.section .data
	.globl _temp0
	.align 4
_temp0:
	.space 8
	.globl _temp1
	.align 4
_temp1:
	.space 1
	.globl _temp10
	.align 4
_temp10:
	.space 1
	.globl _temp11
	.align 4
_temp11:
	.space 4
	.globl _temp12
	.align 4
_temp12:
	.space 8
	.globl _temp13
	.align 4
_temp13:
	.space 1
	.globl _temp14
	.align 4
_temp14:
	.space 1
	.globl _temp15
	.align 4
_temp15:
	.space 4
	.globl _temp16
	.align 4
_temp16:
	.space 1
	.globl _temp17
	.align 4
_temp17:
	.space 4
	.globl _temp18
	.align 4
_temp18:
	.space 1
	.globl _temp19
	.align 4
_temp19:
	.space 4
	.globl _temp2
	.align 4
_temp2:
	.space 8
	.globl _temp20
	.align 4
_temp20:
	.space 4
	.globl _temp21
	.align 4
_temp21:
	.space 4
	.globl _temp22
	.align 4
_temp22:
	.space 4
	.globl _temp23
	.align 4
_temp23:
	.space 4
	.globl _temp24
	.align 4
_temp24:
	.space 4
	.globl _temp25
	.align 4
_temp25:
	.space 4
	.globl _temp26
	.align 4
_temp26:
	.space 4
	.globl _temp27
	.align 4
_temp27:
	.space 1
	.globl _temp28
	.align 4
_temp28:
	.space 1
	.globl _temp29
	.align 4
_temp29:
	.space 4
	.globl _temp3
	.align 4
_temp3:
	.space 8
	.globl _temp30
	.align 4
_temp30:
	.space 4
	.globl _temp31
	.align 4
_temp31:
	.space 4
	.globl _temp32
	.align 4
_temp32:
	.space 1
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
	.space 1
	.globl _temp9
	.align 4
_temp9:
	.space 8
	.globl aux_contador
	.align 4
aux_contador:
	.long 0
	.globl aux_mult
	.align 4
aux_mult:
	.long 2
	.globl aux_retorno_funcoes
	.align 4
aux_retorno_funcoes:
	.long 0
	.globl aux_valor_real
	.align 4
aux_valor_real:
	.long 2
	.long 10
	.globl quantidade
	.align 4
quantidade:
	.space 4
	.globl saida
	.align 4
saida:
	.byte 0
	.globl valor
	.align 4
valor:
	.space 4
	.globl variavel1
	.align 4
variavel1:
	.byte 10
	.globl x
	.align 4
x:
	.long 10
	.globl y
	.align 4
y:
	.byte 0
	.globl z
	.align 4
z:
	.long 1
	.long 10
	.long 2
	.long 10
	.long 3
	.long 10
	.long 4
	.long 10
	.long 5
	.long 10
	.globl matriz_vazia
	.align 4
matriz_vazia:
	.space 40
	.globl valores
	.align 4
valores:
	.byte 0
	.byte 1
	.byte 2
	.byte 3
	.byte 4

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
	.long 0
	.globl LIT2
	.align 4
LIT2:
	.long 2
	.globl LIT3
	.align 4
LIT3:
	.long 2
	.long 10
	.globl LIT4
	.align 4
LIT4:
	.long 5
	.globl LIT5
	.align 4
LIT5:
	.long 1
	.long 10
	.globl LIT6
	.align 4
LIT6:
	.long 3
	.long 10
	.globl LIT7
	.align 4
LIT7:
	.long 4
	.long 10
	.globl LIT8
	.align 4
LIT8:
	.long 5
	.long 10
	.globl LIT9
	.align 4
LIT9:
	.long 1
	.globl LIT10
	.align 4
LIT10:
	.long 3
	.globl LIT11
	.align 4
LIT11:
	.long 4
	.globl LIT12
LIT12:
	.string "\nValor de z[contador]: "
	.globl LIT13
LIT13:
	.string "\nValor de matriz_vazia[contador]: "
	.globl LIT14
LIT14:
	.string "\n"
	.globl LIT15
LIT15:
	.string "o valor de z[contador] eh maior que 1/10!\n"
	.globl LIT16
LIT16:
	.string "o valor de z[contador] nao eh maior que 1/10!\n"
	.globl LIT17
LIT17:
	.string "o valor de z[contador] eh menor que 3/10!\n"
	.globl LIT18
LIT18:
	.string "\Terminando Funcao manipula_real\n"
	.globl LIT19
LIT19:
	.string "\nIniciando Funcao manipula_bytes\n"
	.globl LIT20
LIT20:
	.string "\nValor original da variavel1: "
	.globl LIT21
LIT21:
	.string "\nvariavel1 apos mult: "
	.globl LIT22
LIT22:
	.string "\nvariavel1 apos div: "
	.globl LIT23
LIT23:
	.string "\nvariavel1 apos sub: "
	.globl LIT24
LIT24:
	.string "\Terminando Funcao manipula_bytes\n"
	.globl LIT25
	.align 4
LIT25:
	.long 8
	.globl LIT26
LIT26:
	.string "\nIniciando Funcao main\n"
	.globl LIT27
LIT27:
	.string "Valor da variavel x: "
	.globl LIT28
	.align 4
LIT28:
	.long 12
	.globl LIT29
LIT29:
	.string "\nColoque um valor pra variavel y: "
	.globl LIT30
LIT30:
	.string "Printando valor lido: "
	.globl LIT31
LIT31:
	.string "\nExecutando funcao manipula_real"
	.globl LIT32
LIT32:
	.string "\nIniciando Funcao multiplica\n"
	.globl LIT33
LIT33:
	.string "valor da variavel quantidade antes do while loop: "
	.globl LIT34
	.align 4
LIT34:
	.long 50
	.globl LIT35
LIT35:
	.string "valor atingido: "
	.globl LIT36
LIT36:
	.string "valor maior que 50\n"
	.globl LIT37
LIT37:
	.string "valor da variavel quantidade no loop: "
	.globl LIT38
LIT38:
	.string "valor da variavel quantidade depois do loop: "
	.globl LIT39
LIT39:
	.string "\Terminando Funcao multiplica\n"
	.section .text
	.globl main
manipula_real:
	pushq %rbp
	movq %rsp, %rbp
_label3:
	# Lendo elemento de vetor: z[4]
	movl	LIT11(%rip), %ecx
	leaq	z(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp0(%rip)
	# Real EQUAL
	movl	_temp0(%rip), %r8d
	movl	4+_temp0(%rip), %r9d
	movl	LIT8(%rip), %r10d
	movl	4+LIT8(%rip), %r11d
	movslq	%r8d, %rax
	imulq	%r11, %rax
	movslq	%r10d, %rbx
	imulq	%r9, %rbx
	cmpq	%rbx, %rax
	sete	%al
	movb	%al, _temp1(%rip)
	# Salto condicional pra _label4
	movzbl	_temp1(%rip), %eax
	testl	%eax, %eax
	je	_label4
	# Lendo elemento de vetor: z[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	z(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp2(%rip)
	# Atribuindo a elemento de vetor: z[aux_contador] = aux_valor_real
	movl	aux_contador(%rip), %ecx
	leaq	z(%rip), %rax
	movl	aux_valor_real(%rip), %edx
	movl	4+aux_valor_real(%rip), %r8d
	movl	%edx, (%rax, %rcx, 8)
	movl	%r8d, 4(%rax, %rcx, 8)
	# Lendo elemento de vetor: matriz_vazia[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	matriz_vazia(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp3(%rip)
	# Lendo elemento de vetor: z[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	z(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp4(%rip)
	# Atribuindo a elemento de vetor: matriz_vazia[aux_contador] = _temp4
	movl	aux_contador(%rip), %ecx
	leaq	matriz_vazia(%rip), %rax
	movl	_temp4(%rip), %edx
	movl	4+_temp4(%rip), %r8d
	movl	%edx, (%rax, %rcx, 8)
	movl	%r8d, 4(%rax, %rcx, 8)
	# Lendo elemento de vetor: z[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	z(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp5(%rip)
	# Imprimindo valor: "\nValor de z[contador]: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT12(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: _temp5
	movl	_temp5(%rip), %esi
	movl	4+_temp5(%rip), %edx
	leaq	.LC_REAL(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Lendo elemento de vetor: matriz_vazia[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	matriz_vazia(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp6(%rip)
	# Imprimindo valor: "\nValor de matriz_vazia[contador]: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT13(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: _temp6
	movl	_temp6(%rip), %esi
	movl	4+_temp6(%rip), %edx
	leaq	.LC_REAL(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo elemento de vetor: z[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	z(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp7(%rip)
	# Real GREATER
	movl	_temp7(%rip), %r8d
	movl	4+_temp7(%rip), %r9d
	movl	LIT5(%rip), %r10d
	movl	4+LIT5(%rip), %r11d
	movslq	%r8d, %rax
	imulq	%r11, %rax
	movslq	%r10d, %rbx
	imulq	%r9, %rbx
	cmpq	%rbx, %rax
	setg	%al
	movb	%al, _temp8(%rip)
	# Salto condicional pra _label0
	movzbl	_temp8(%rip), %eax
	testl	%eax, %eax
	je	_label0
	# Imprimindo valor: "o valor de z[contador] eh maior que 1/10!\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Salto incondicional pra _label1
	jmp	_label1
_label0:
	# Imprimindo valor: "o valor de z[contador] nao eh maior que 1/10!\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT16(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
_label1:
	# Lendo elemento de vetor: z[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	z(%rip), %rax
	movq	(%rax, %rcx, 8), %rbx
	movq	%rbx, _temp9(%rip)
	# Real LESS
	movl	_temp9(%rip), %r8d
	movl	4+_temp9(%rip), %r9d
	movl	LIT6(%rip), %r10d
	movl	4+LIT6(%rip), %r11d
	movslq	%r8d, %rax
	imulq	%r11, %rax
	movslq	%r10d, %rbx
	imulq	%r9, %rbx
	cmpq	%rbx, %rax
	setl	%al
	movb	%al, _temp10(%rip)
	# Salto condicional pra _label2
	movzbl	_temp10(%rip), %eax
	testl	%eax, %eax
	je	_label2
	# Imprimindo valor: "o valor de z[contador] eh menor que 3/10!\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT17(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
_label2:
	# Int ADD
	movl	aux_contador(%rip), %eax
	movl	LIT9(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp11(%rip)
	# Atribuição INT
	movl	_temp11(%rip), %eax
	movl	%eax, aux_contador(%rip)
	# Real ADD
	movl	aux_valor_real(%rip), %r8d
	movl	4+aux_valor_real(%rip), %r9d
	movl	LIT5(%rip), %r10d
	movl	4+LIT5(%rip), %r11d
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
	movl	%eax, _temp12(%rip)
	movl	%edx, 4+_temp12(%rip)
	# Atribuição REAL
	movl	_temp12(%rip), %eax
	movl	4+_temp12(%rip), %edx
	movl	%eax, aux_valor_real(%rip)
	movl	%edx, 4+aux_valor_real(%rip)
	# Salto condicional pra _label3
	movzbl	_temp1(%rip), %eax
	testl	%eax, %eax
	jne	_label3
_label4:
	# Imprimindo valor: "\Terminando Funcao manipula_real\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT18(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Retornando INT
	movl	LIT9(%rip), %eax
	# Saindo da função por meio do return
	leave
	ret
	# Fim da função manipula_real
	leave
	ret
manipula_bytes:
	pushq %rbp
	movq %rsp, %rbp
	# Atribuição INT
	movl	LIT1(%rip), %eax
	movl	%eax, aux_contador(%rip)
	# Imprimindo valor: "\nIniciando Funcao manipula_bytes\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT19(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
_label5:
	# Lendo elemento de vetor: valores[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	valores(%rip), %rax
	movzbl	(%rax, %rcx, 1), %edx
	movl	%edx, _temp13(%rip)
	# Lendo elemento de vetor: valores[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	valores(%rip), %rax
	movzbl	(%rax, %rcx, 1), %edx
	movl	%edx, _temp14(%rip)
	# Byte ADD
	movb	_temp14(%rip), %al
	movb	LIT0(%rip), %bl
	addb	%bl, %al
	movb	%al, _temp15(%rip)
	# Atribuindo a elemento de vetor: valores[aux_contador] = _temp15
	movl	aux_contador(%rip), %ecx
	leaq	valores(%rip), %rax
	movb	_temp15(%rip), %dl
	movb	%dl, (%rax, %rcx, 1)
	# Lendo elemento de vetor: valores[aux_contador]
	movl	aux_contador(%rip), %ecx
	leaq	valores(%rip), %rax
	movzbl	(%rax, %rcx, 1), %edx
	movl	%edx, _temp16(%rip)
	# Imprimindo valor: _temp16
	movzbl	_temp16(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Int ADD
	movl	aux_contador(%rip), %eax
	movl	LIT9(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp17(%rip)
	# Atribuição INT
	movl	_temp17(%rip), %eax
	movl	%eax, aux_contador(%rip)
	# Int LESS
	movl	aux_contador(%rip), %eax
	movl	LIT4(%rip), %edx
	cmpl	%edx, %eax
	setl	%al
	movb	%al, _temp18(%rip)
	# Salto condicional pra _label5
	movzbl	_temp18(%rip), %eax
	testl	%eax, %eax
	jne	_label5
	# Imprimindo valor: "\nValor original da variavel1: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT20(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Byte MUL
	movb	variavel1(%rip), %al
	imulb	aux_contador(%rip)
	movb	%al, _temp19(%rip)
	# Atribuição INT
	movl	_temp19(%rip), %eax
	movl	%eax, variavel1(%rip)
	# Imprimindo valor: "\nvariavel1 apos mult: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT21(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Byte DIV
	movb	variavel1(%rip), %al
	cbw
	idivb	LIT0(%rip)
	movb	%al, _temp20(%rip)
	# Atribuição INT
	movl	_temp20(%rip), %eax
	movl	%eax, variavel1(%rip)
	# Imprimindo valor: "\nvariavel1 apos div: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT22(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Byte SUB
	movb	variavel1(%rip), %al
	movb	LIT4(%rip), %bl
	subb	%bl, %al
	movb	%al, _temp21(%rip)
	# Atribuição INT
	movl	_temp21(%rip), %eax
	movl	%eax, variavel1(%rip)
	# Imprimindo valor: "\nvariavel1 apos sub: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT23(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\Terminando Funcao manipula_bytes\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT24(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Retornando INT
	movl	LIT25(%rip), %eax
	# Saindo da função por meio do return
	leave
	ret
	# Fim da função manipula_bytes
	leave
	ret
main:
	pushq %rbp
	movq %rsp, %rbp
	# Imprimindo valor: "\nIniciando Funcao main\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT26(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Int ADD
	movl	LIT4(%rip), %eax
	movl	LIT2(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp22(%rip)
	# Int DIV
	movl	LIT0(%rip), %eax
	movl	LIT2(%rip), %ecx
	xorl	%edx, %edx
	cltd
	idivl	%ecx
	movl	%eax, _temp23(%rip)
	# Int SUB
	movl	_temp22(%rip), %eax
	movl	_temp23(%rip), %edx
	subl	%edx, %eax
	movl	%eax, _temp24(%rip)
	# Atribuição INT
	movl	_temp24(%rip), %eax
	movl	%eax, x(%rip)
	# Imprimindo valor: "Valor da variavel x: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT27(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: x
	movl	x(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Preparando argumentos pra chamada de multiplica
	movl	LIT28(%rip), %edi
	movl	LIT2(%rip), %esi
	call	multiplica
	# Armazenando valor de retorno de multiplica em _temp25
	movl	%eax, _temp25(%rip)
	# Atribuição INT
	movl	_temp25(%rip), %eax
	movl	%eax, aux_retorno_funcoes(%rip)
	# Imprimindo valor: aux_retorno_funcoes
	movl	aux_retorno_funcoes(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nColoque um valor pra variavel y: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT29(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel y
	call	_read_and_convert
	movb	%al, y(%rip)
	# Imprimindo valor: "Printando valor lido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT30(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: y
	movzbl	y(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nExecutando funcao manipula_real"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT31(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Preparando argumentos pra chamada de manipula_real
	call	manipula_real
	# Armazenando valor de retorno de manipula_real em _temp26
	movl	%eax, _temp26(%rip)
	# Atribuição INT
	movl	_temp26(%rip), %eax
	movl	%eax, aux_mult(%rip)
	# Imprimindo valor: aux_mult
	movl	aux_mult(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Preparando argumentos pra chamada de manipula_bytes
	call	manipula_bytes
	# Armazenando valor de retorno de manipula_bytes em _temp27
	movb	%al, _temp27(%rip)
	# Atribuição BYTE
	movb	_temp27(%rip), %al
	movb	%al, saida(%rip)
	# Imprimindo valor: saida
	movzbl	saida(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Fim da função main
	leave
	ret
multiplica:
	pushq %rbp
	movq %rsp, %rbp
	# Movendo argumento 1 (quantidade) do registrador pra variavel global
	movl	%edi, quantidade(%rip)
	# Movendo argumento 2 (valor) do registrador pra variavel global
	movl	%esi, valor(%rip)
	# Imprimindo valor: "\nIniciando Funcao multiplica\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT32(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor da variavel quantidade antes do while loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT33(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
_label8:
	# Int GREATER
	movl	valor(%rip), %eax
	movl	LIT34(%rip), %edx
	cmpl	%edx, %eax
	setg	%al
	movb	%al, _temp28(%rip)
	# Salto condicional pra _label6
	movzbl	_temp28(%rip), %eax
	testl	%eax, %eax
	je	_label6
	# Int MUL
	movl	valor(%rip), %eax
	imull	aux_mult(%rip), %eax
	movl	%eax, _temp29(%rip)
	# Atribuição INT
	movl	_temp29(%rip), %eax
	movl	%eax, valor(%rip)
	# Imprimindo valor: "valor atingido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT35(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: valor
	movl	valor(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor maior que 50\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT36(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Salto incondicional pra _label7
	jmp	_label7
_label6:
	# Int MUL
	movl	valor(%rip), %eax
	imull	aux_mult(%rip), %eax
	movl	%eax, _temp30(%rip)
	# Atribuição INT
	movl	_temp30(%rip), %eax
	movl	%eax, valor(%rip)
_label7:
	# Imprimindo valor: "valor da variavel quantidade no loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT37(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Int SUB
	movl	quantidade(%rip), %eax
	movl	LIT9(%rip), %edx
	subl	%edx, %eax
	movl	%eax, _temp31(%rip)
	# Atribuição INT
	movl	_temp31(%rip), %eax
	movl	%eax, quantidade(%rip)
	# Int LESS
	movl	quantidade(%rip), %eax
	movl	LIT4(%rip), %edx
	cmpl	%edx, %eax
	setl	%al
	movb	%al, _temp32(%rip)
	# Salto condicional pra _label8
	movzbl	_temp32(%rip), %eax
	testl	%eax, %eax
	jne	_label8
	# Imprimindo valor: "valor da variavel quantidade depois do loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT38(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor atingido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT35(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: valor
	movl	valor(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT14(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\Terminando Funcao multiplica\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT39(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Retornando INT
	movl	valor(%rip), %eax
	# Saindo da função por meio do return
	leave
	ret
	# Fim da função multiplica
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
