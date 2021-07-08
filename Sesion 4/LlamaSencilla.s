#int LlamaSencilla(S1 x, char y) {
#	*(x.m)=SencillaSub(x,x.c);
	#return x.k;
#}

LlamaSencilla:
	pushl %ebp
	movl %esp, %ebp
	pushb 8(%ebp)	#push x.c

	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)	#push x

	call SencillaSub
	addl $16,%esp	#recuperamos el espacio de los push

	#el valor del return está en %eax
	movl 16(%ebp), %edx	#x.m
	movl %eax, (%edx)	#*(x.m) = %eax (return de la SencillaSub)

	movl 12(%ebp), %eax	#return x.k
	movl %ebp, %esp
	popl %ebp
	ret