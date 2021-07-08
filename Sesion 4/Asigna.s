 .text
	.align 4
	.globl Asignar
	.type Asignar,@function
Asignar:
	pushl %ebp
	movl %esp,%ebp

#tamElem = 12bytes
#v[pos] = @v[0]+pos*tamElem = 24(%ebp) * 12 + 8(%ebp)

#x.c = 12(%ebp); x.k = 16(%ebp); 20(%ebp) 

#pos = 24(%ebp)

    #carreguem registres a la pila
    subl $8, %esp 
	pushl %ebx
	pushl %edi
	
	imull $12, 24(%ebp), %edi	#pos*tamElem
	addl 8(%ebp), %edi		#@v[0]+pos*12 = @v[pos]

	movb 12(%ebp), %bl		#x.c
	movb %bl, (%edi)	#v[pos].c = x.c
	movl 16(%ebp), %ebx		#x.k
	movl %ebx, 4(%edi)	#v[pos].k = x.k
	movl 20(%ebp), %ebx		#x.m
	movl %ebx, 8(%edi)	#v[pos].k = x.m
	
	#recuperem registres de la pila
	popl %edi
	popl %ebx
	movl %ebp, %esp
	popl %ebp
	ret
