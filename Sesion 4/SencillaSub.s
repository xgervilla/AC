#int SencillaSub(S1 a, char b) {
#	int i;
#	if (a.c==b)
#		i=0;
#	else
#		i=*(a.m);
#	return i+a.k
#}

#%esi, %ebx, %edi se tienen que guadar si son modificadas
#valor de return se guarda en %eax

#i,%ebp,@RET,@a,b

#a.c = (@a[0])
#*(a.m) = ( (@a[0]+8) )

SencillaSub:
	pushl %ebp
	movl %esp, %ebp
	subl $4, %esp	#i
	movl 8(%ebp), %eax	#@a[0]
	movl (%eax),%cx	#a.c
#if:
	movl 12(%ebp), %dx	#b
	cmpl %cx, %dx
	jne else
	movl $0, -4(%ebp)	#i = 0
	jmp suma_return
else:
	movl 8(%eax), %ecx	#(@a[0]+8) == a.m
	movl (%ecx), %ecx	#*(a.m)
	movl %ecx, -4(%ebp)
suma_return:
	movl 4(%eax), %eax	#a.k
	addl -4(%ebp), %eax	#i+a.k
#preparem return i tornem
	movl %ebp, %esp
	popl %ebp
	ret
