.text
    .align 4
    .globl OperaMat
    .type    OperaMat, @function
OperaMat:
    pushl    %ebp
    movl    %esp, %ebp
    subl    $16, %esp
    pushl    %ebx
    pushl    %esi
    pushl    %edi
# Aqui has de introducir el codigo
    movl $0, -4(%ebp)   #res=0
    movl $0, %eax       #i
    
for_i:
    cmpl $3, %eax
    jge fi_for_i
    movl $0, %ebx       #si i == 3 entonces sale del bucle, sino cargamos un 0 en ebx (j) para entrar al segundo for
    
for_j:
    cmpl %eax, %ebx
    jg fi_for_j
    
    #hay que acceder a M[i][j] == base+4*(i*Ncol+j)
    imull $3, %eax, %edx    #i*Ncol
    addl %ebx, %edx          #i*Ncol+j
    movl 8(%ebp), %ecx      #base de la matriz (@M[0][0])
    
    movl (%ecx, %edx, 4), %edx    #M[i][j] == base+ 4*(i*Ncol+j)
    addl %edx, -4(%ebp)         #res+=M[i][j]
    
    incl %ebx               #j++
    jmp for_j
    
fi_for_j:
    addl 12(%ebp), %eax #i+=salto (salto esta en 12(%ebp))
    jmp for_i           #vuelve al bucle del for i

fi_for_i:
# El final de la rutina ya esta programado
    movl    -4(%ebp), %eax
    popl    %edi
    popl    %esi
    popl    %ebx
    movl %ebp,%esp
    popl %ebp
    ret
