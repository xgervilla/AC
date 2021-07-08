#Xavier Gervilla Machado i Carlos Exojo Gavilan, grup 23

.text
	.align 16
	.globl procesar
	.type	procesar, @function
un: .byte 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
cero: .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

procesar:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi

# Aqui has de introducir el codigo
    movl $0, %esi         #contador (i)
    movl 8(%ebp), %eax    #mata
    movl 12(%ebp), %ebx   #matb
    movl 16(%ebp), %ecx   #n
    imul %ecx, %ecx       #n*n

for:
    cmpl %ecx, %esi     #comparamos i con n*n
    jge fifor
    
    movdqa un, %xmm0        #cargamos los 1s
    movdqa (%eax, %esi), %xmm1  #cargamos mata
    pand %xmm1, %xmm0    #hacemos el AND con los 1s
    
    movdqa cero, %xmm1      #cargamos los 0s
    pcmpgtb %xmm1, %xmm0           #comparamos
    
    movdqa %xmm0, (%ebx, %esi)    #guardamos el resultado en matb (0xFF o 0x00)
    
    addl $16, %esi                     #incrementamos esi (i+=16)
    jmp for
fifor:
# El final de la rutina ya esta programado

	emms	# Instruccion necesaria si os equivocais y usais MMX
	popl	%edi
	popl	%esi
	popl	%ebx
	movl %ebp,%esp
	popl %ebp
	ret
