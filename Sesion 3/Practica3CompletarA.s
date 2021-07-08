.text
    .align 4
    .globl OperaVec
    .type    OperaVec, @function
OperaVec:
    pushl    %ebp
    movl    %esp, %ebp
    subl    $16, %esp     #aumenta la pila en 16 (4 int)
    pushl    %ebx          #guarda ebx, esi y edi en la pila
    pushl    %esi
    pushl    %edi
    movl    8(%ebp), %eax #@Vector[0]
    movl    (%eax), %eax  #Vector[0]
    movl    %eax, -4(%ebp) #res = Vector[0]
# Aqui has de introducir el codigo

    #movl -8(%ebp), %ebx       #en principio i está en esa posición de memoria pero da segmentation fault
    movl $1, %ebx
for:
    cmpl 12(%ebp), %ebx #elementos esta en 12(%ebp)
    je fi_for
    
    #cargamos Vector[i-1] y Vector [i]
    movl 8(%ebp), %eax              #base del vector
    movl (%eax, %ebx, 4), %ecx      #vector[i]
    movl -4(%eax, %ebx, 4), %edx    #vector[i-1]
    cmpl %ecx, %edx
    jne else
    movl %ebx, -4(%ebp)
    
else:
    incl %ebx
    jmp for
    
fi_for:
# El final de la rutina ya esta programado
    movl    -4(%ebp), %eax    #mueve res al registro de retorno (eax)
    popl    %edi          #recupera los registros de la pila
    popl    %esi
    popl    %ebx
    movl %ebp,%esp        #recupera el valor original de %esp
    popl %ebp
    ret
