	.section	.note.GNU-stack,"",@progbits
	.section .data
	.comm _temp0, 8, 4
	.comm _temp1, 1, 4
	.comm _temp10, 1, 4
	.comm _temp11, 4, 4
	.comm _temp12, 8, 4
	.comm _temp13, 1, 4
	.comm _temp14, 1, 4
	.comm _temp15, 4, 4
	.comm _temp16, 1, 4
	.comm _temp17, 4, 4
	.comm _temp18, 1, 4
	.comm _temp19, 4, 4
	.comm _temp2, 8, 4
	.comm _temp20, 4, 4
	.comm _temp21, 4, 4
	.comm _temp22, 1, 4
	.comm _temp23, 4, 4
	.comm _temp24, 1, 4
	.comm _temp25, 4, 4
	.comm _temp26, 1, 4
	.comm _temp27, 1, 4
	.comm _temp28, 4, 4
	.comm _temp29, 4, 4
	.comm _temp3, 8, 4
	.comm _temp30, 4, 4
	.comm _temp31, 4, 4
	.comm _temp32, 1, 4
	.comm _temp33, 4, 4
	.comm _temp34, 4, 4
	.comm _temp35, 4, 4
	.comm _temp36, 1, 4
	.comm _temp4, 8, 4
	.comm _temp5, 8, 4
	.comm _temp6, 8, 4
	.comm _temp7, 8, 4
	.comm _temp8, 1, 4
	.comm _temp9, 8, 4
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
	.globl test_function
	.align 4
test_function:
	.long 0
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
	.comm matriz_vazia, 40, 4
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
.LC_ERR_INT_FORMAT:
	.string "Erro de execucao: formato de entrada invalido para read (esperado um inteiro).\n"
.LC_ERR_REAL_FORMAT:
	.string "Erro de execucao: formato de entrada invalido para read (esperado um real no formato num/den).\n"
	.comm .read_int_buffer, 4, 4
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
	.string "\nIniciando Funcao manipula_real\n"
	.globl LIT13
LIT13:
	.string "\nValor de z[contador]: "
	.globl LIT14
LIT14:
	.string "\nValor de matriz_vazia[contador]: "
	.globl LIT15
LIT15:
	.string "\n"
	.globl LIT16
LIT16:
	.string "o valor de z[contador] eh maior que 1/10!\n"
	.globl LIT17
LIT17:
	.string "o valor de z[contador] nao eh maior que 1/10!\n"
	.globl LIT18
LIT18:
	.string "o valor de z[contador] eh menor que 3/10!\n"
	.globl LIT19
LIT19:
	.string "\Terminando Funcao manipula_real\n"
	.globl LIT20
LIT20:
	.string "\nIniciando Funcao manipula_bytes\n"
	.globl LIT21
LIT21:
	.string "valor atual de valores[aux_contador]: "
	.globl LIT22
LIT22:
	.string "\nValor original da variavel1: "
	.globl LIT23
LIT23:
	.string "\nvariavel1 apos mult: "
	.globl LIT24
LIT24:
	.string "\nvariavel1 apos div: "
	.globl LIT25
LIT25:
	.string "\nvariavel1 apos sub: "
	.globl LIT26
LIT26:
	.string "\Terminando Funcao manipula_bytes\n"
	.globl LIT27
	.align 4
LIT27:
	.long 8
	.globl LIT28
LIT28:
	.string "\nIniciando Funcao main"
	.globl LIT29
LIT29:
	.string "\nDeseja executar funcao multiplica? Digite 1 pra executar e 0 pra nao executar: "
	.globl LIT30
LIT30:
	.string "\nValor de saida da funcao multiplica: "
	.globl LIT31
LIT31:
	.string "\nDeseja executar funcao manipula_real? Digite 1 pra executar e 0 pra nao executar: "
	.globl LIT32
LIT32:
	.string "\nValor de saida da funcao manipula_real: "
	.globl LIT33
LIT33:
	.string "\nDeseja executar funcao manipula_bytes? Digite 1 pra executar e 0 pra nao executar: "
	.globl LIT34
LIT34:
	.string "\nValor de saida da funcao manipula_bytes: "
	.globl LIT35
LIT35:
	.string "\nValor da variavel x antes das operacoes: "
	.globl LIT36
LIT36:
	.string "\nValor da variavel x apos operacoes: "
	.globl LIT37
LIT37:
	.string "\nColoque um valor int pra variavel x: "
	.globl LIT38
LIT38:
	.string "Printando valor lido: "
	.globl LIT39
LIT39:
	.string "\nColoque um valor int (ate 255) pra variavel y: "
	.globl LIT40
LIT40:
	.string "\nColoque um valor real pra variavel aux_valor_real: "
	.globl LIT41
LIT41:
	.string "\nIniciando Funcao multiplica"
	.globl LIT42
LIT42:
	.string "\nvalor da variavel quantidade antes do while loop: "
	.globl LIT43
	.align 4
LIT43:
	.long 50
	.globl LIT44
LIT44:
	.string "valor atingido: "
	.globl LIT45
LIT45:
	.string "valor maior que 50\n"
	.globl LIT46
LIT46:
	.string "valor da variavel quantidade no loop: "
	.globl LIT47
LIT47:
	.string "valor da variavel quantidade depois do loop: "
	.globl LIT48
LIT48:
	.string "\Terminando Funcao multiplica\n"
	.section .text
	.globl main
manipula_real:
	pushq %rbp
	movq %rsp, %rbp
	# Imprimindo valor: "\nIniciando Funcao manipula_real\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT12(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
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
	cmpb	$0, _temp1(%rip)
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
	leaq	LIT13(%rip), %rsi
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
	leaq	LIT14(%rip), %rsi
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
	leaq	LIT15(%rip), %rsi
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
	cmpb	$0, _temp8(%rip)
	je	_label0
	# Imprimindo valor: "o valor de z[contador] eh maior que 1/10!\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT16(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Salto incondicional pra _label1
	jmp	_label1
_label0:
	# Imprimindo valor: "o valor de z[contador] nao eh maior que 1/10!\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT17(%rip), %rsi
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
	cmpb	$0, _temp10(%rip)
	je	_label2
	# Imprimindo valor: "o valor de z[contador] eh menor que 3/10!\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT18(%rip), %rsi
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
	cmpb	$0, _temp1(%rip)
	jne	_label3
_label4:
	# Imprimindo valor: "\Terminando Funcao manipula_real\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT19(%rip), %rsi
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
	leaq	LIT20(%rip), %rsi
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
	# Imprimindo valor: "valor atual de valores[aux_contador]: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT21(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: _temp16
	movzbl	_temp16(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
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
	cmpb	$0, _temp18(%rip)
	jne	_label5
	# Imprimindo valor: "\nValor original da variavel1: "
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
	leaq	LIT15(%rip), %rsi
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
	leaq	LIT23(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
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
	leaq	LIT24(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
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
	leaq	LIT25(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: variavel1
	movzbl	variavel1(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\Terminando Funcao manipula_bytes\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT26(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Retornando INT
	movl	LIT27(%rip), %eax
	# Saindo da função por meio do return
	leave
	ret
	# Fim da função manipula_bytes
	leave
	ret
main:
	pushq %rbp
	movq %rsp, %rbp
	# Imprimindo valor: "\nIniciando Funcao main"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT28(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nDeseja executar funcao multiplica? Digite 1 pra executar e 0 pra nao executar: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT29(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel test_function
	call	_read_and_convert
	movl	%eax, test_function(%rip)
	# Int EQUAL
	movl	test_function(%rip), %eax
	movl	LIT9(%rip), %edx
	cmpl	%edx, %eax
	sete	%al
	movb	%al, _temp22(%rip)
	# Salto condicional pra _label6
	cmpb	$0, _temp22(%rip)
	je	_label6
	# Preparando argumentos pra chamada de multiplica
	movl	LIT2(%rip), %edi
	movl	LIT2(%rip), %esi
	call	multiplica
	# Armazenando valor de retorno de multiplica em _temp23
	movl	%eax, _temp23(%rip)
	# Atribuição INT
	movl	_temp23(%rip), %eax
	movl	%eax, aux_retorno_funcoes(%rip)
	# Imprimindo valor: "\nValor de saida da funcao multiplica: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT30(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: aux_retorno_funcoes
	movl	aux_retorno_funcoes(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
_label6:
	# Imprimindo valor: "\nDeseja executar funcao manipula_real? Digite 1 pra executar e 0 pra nao executar: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT31(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel test_function
	call	_read_and_convert
	movl	%eax, test_function(%rip)
	# Int EQUAL
	movl	test_function(%rip), %eax
	movl	LIT9(%rip), %edx
	cmpl	%edx, %eax
	sete	%al
	movb	%al, _temp24(%rip)
	# Salto condicional pra _label7
	cmpb	$0, _temp24(%rip)
	je	_label7
	# Preparando argumentos pra chamada de manipula_real
	call	manipula_real
	# Armazenando valor de retorno de manipula_real em _temp25
	movl	%eax, _temp25(%rip)
	# Atribuição INT
	movl	_temp25(%rip), %eax
	movl	%eax, aux_retorno_funcoes(%rip)
	# Imprimindo valor: "\nValor de saida da funcao manipula_real: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT32(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: aux_retorno_funcoes
	movl	aux_retorno_funcoes(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
_label7:
	# Imprimindo valor: "\nDeseja executar funcao manipula_bytes? Digite 1 pra executar e 0 pra nao executar: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT33(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel test_function
	call	_read_and_convert
	movl	%eax, test_function(%rip)
	# Int EQUAL
	movl	test_function(%rip), %eax
	movl	LIT9(%rip), %edx
	cmpl	%edx, %eax
	sete	%al
	movb	%al, _temp26(%rip)
	# Salto condicional pra _label8
	cmpb	$0, _temp26(%rip)
	je	_label8
	# Preparando argumentos pra chamada de manipula_bytes
	call	manipula_bytes
	# Armazenando valor de retorno de manipula_bytes em _temp27
	movb	%al, _temp27(%rip)
	# Atribuição BYTE
	movb	_temp27(%rip), %al
	movb	%al, saida(%rip)
	# Imprimindo valor: "\nValor de saida da funcao manipula_bytes: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT34(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: saida
	movzbl	saida(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
_label8:
	# Imprimindo valor: "\nValor da variavel x antes das operacoes: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT35(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: x
	movl	x(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Int MUL (otimizado com SHL)
	movl	LIT4(%rip), %eax
	shll	$1, %eax
	movl	%eax, _temp28(%rip)
	# Int ADD
	movl	_temp28(%rip), %eax
	movl	LIT2(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp29(%rip)
	# Int DIV
	movl	LIT0(%rip), %eax
	movl	LIT2(%rip), %ecx
	xorl	%edx, %edx
	cltd
	idivl	%ecx
	movl	%eax, _temp30(%rip)
	# Int SUB
	movl	_temp29(%rip), %eax
	movl	_temp30(%rip), %edx
	subl	%edx, %eax
	movl	%eax, _temp31(%rip)
	# Atribuição INT
	movl	_temp31(%rip), %eax
	movl	%eax, x(%rip)
	# Imprimindo valor: "\nValor da variavel x apos operacoes: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT36(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: x
	movl	x(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nColoque um valor int pra variavel x: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT37(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel x
	call	_read_and_convert
	movl	%eax, x(%rip)
	# Imprimindo valor: "Printando valor lido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT38(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: x
	movl	x(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nColoque um valor int (ate 255) pra variavel y: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT39(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel y
	call	_read_and_convert
	movb	%al, y(%rip)
	# Imprimindo valor: "Printando valor lido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT38(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: y
	movzbl	y(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nColoque um valor real pra variavel aux_valor_real: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT40(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Lendo valor para a variavel aux_valor_real
	leaq	.LC_SCAN_REAL(%rip), %rdi
	leaq	aux_valor_real(%rip), %rsi
	leaq	4+aux_valor_real(%rip), %rdx
	movl	$0, %eax
	call	scanf@PLT
	cmpl	$2, %eax
	je	.L_read_ok_0x57e34b4e75c0
.L_read_error_real_0x57e34b4e75c0:
	leaq	.LC_ERR_REAL_FORMAT(%rip), %rdi
	call	_runtime_error
.L_read_ok_0x57e34b4e75c0:
	# Imprimindo valor: "Printando valor lido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT38(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: aux_valor_real
	movl	aux_valor_real(%rip), %esi
	movl	4+aux_valor_real(%rip), %edx
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
multiplica:
	pushq %rbp
	movq %rsp, %rbp
	# Movendo argumento 1 (quantidade) do registrador pra variavel global
	movl	%edi, quantidade(%rip)
	# Movendo argumento 2 (valor) do registrador pra variavel global
	movl	%esi, valor(%rip)
	# Imprimindo valor: "\nIniciando Funcao multiplica"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT41(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\nvalor da variavel quantidade antes do while loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT42(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
_label11:
	# Int GREATER
	movl	valor(%rip), %eax
	movl	LIT43(%rip), %edx
	cmpl	%edx, %eax
	setg	%al
	movb	%al, _temp32(%rip)
	# Salto condicional pra _label9
	cmpb	$0, _temp32(%rip)
	je	_label9
	# Int MUL (otimizado com SHL)
	movl	valor(%rip), %eax
	shll	$1, %eax
	movl	%eax, _temp33(%rip)
	# Atribuição INT
	movl	_temp33(%rip), %eax
	movl	%eax, valor(%rip)
	# Imprimindo valor: "valor atingido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT44(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: valor
	movl	valor(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor maior que 50\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT45(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Salto incondicional pra _label10
	jmp	_label10
_label9:
	# Int MUL (otimizado com SHL)
	movl	valor(%rip), %eax
	shll	$1, %eax
	movl	%eax, _temp34(%rip)
	# Atribuição INT
	movl	_temp34(%rip), %eax
	movl	%eax, valor(%rip)
_label10:
	# Imprimindo valor: "valor da variavel quantidade no loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT46(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Int ADD
	movl	quantidade(%rip), %eax
	movl	LIT9(%rip), %edx
	addl	%edx, %eax
	movl	%eax, _temp35(%rip)
	# Atribuição INT
	movl	_temp35(%rip), %eax
	movl	%eax, quantidade(%rip)
	# Int LESS
	movl	quantidade(%rip), %eax
	movl	LIT4(%rip), %edx
	cmpl	%edx, %eax
	setl	%al
	movb	%al, _temp36(%rip)
	# Salto condicional pra _label11
	cmpb	$0, _temp36(%rip)
	jne	_label11
	# Imprimindo valor: "valor da variavel quantidade depois do loop: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT47(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: quantidade
	movl	quantidade(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "valor atingido: "
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT44(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: valor
	movl	valor(%rip), %esi
	leaq	.LC_INT(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT15(%rip), %rsi
	movl	$0, %eax
	call	printf@PLT
	# Imprimindo valor: "\Terminando Funcao multiplica\n"
	leaq	.LC_STRING(%rip), %rdi
	leaq	LIT48(%rip), %rsi
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
	leaq	.read_buffer(%rip), %rcx
	movzbl	(%rcx), %eax
	testb	%al, %al
	je	.L_read_int_error
	movzbl	1(%rcx), %eax
	testb	%al, %al
	je	.L_is_single_char
.L_is_multi_char:
	movzbl	(%rcx), %eax
	cmpb	$'+', %al
	je	.L_skip_sign
	cmpb	$'-', %al
	je	.L_skip_sign
	jmp	.L_check_digit_loop
.L_skip_sign:
	incq	%rcx
	movzbl	(%rcx), %eax
	testb	%al, %al
	je	.L_read_int_error
.L_check_digit_loop:
	movzbl	(%rcx), %eax
	testb	%al, %al
	je	.L_convert_with_atoi
	cmpb	$'0', %al
	jl	.L_read_int_error
	cmpb	$'9', %al
	jg	.L_read_int_error
	incq	%rcx
	jmp	.L_check_digit_loop
.L_is_single_char:
	movzbl	.read_buffer(%rip), %eax
	jmp	.L_read_done
.L_read_int_error:
	leaq	.LC_ERR_INT_FORMAT(%rip), %rdi
	call	_runtime_error
.L_convert_with_atoi:
	leaq	.read_buffer(%rip), %rdi
	call	atoi@PLT
.L_read_done:
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
_runtime_error:
	movq	%rdi, %rsi
	leaq	.LC_STRING(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	exit@PLT
