#----#1.1 y 1.2#----#

#iterativo  (1.1)
    movl $0, %eax       #eax == i
    movl $0, %ebx       #ebx == suma
    movl $Matriz, %ecx  #ecx == Matriz[0][0]
    #hay que acceder a @Matriz[0][0]+(i*Ncol+2)*4
    
for:
    cmpl $10, %eax         #comparamos si i<10
    jge fi_for              #saltamos si i>=10
    
    imull $10,%eax,%edx     #preparamos el acceso (i*10)
    addl 8(%ecx,%edx,4),%ebx #suma+=Matriz[i][2] (base+i*10*4+8)
    
    incl %eax               #incrementamos i
    
    jmp for                 #repetimos el for (nueva iteración)
fi_for:

#Ninst: 3 (preparacion de datos) + 6 * 10 (bucle) = 63

#---------------------------------------------------#

#secuencial (1.2)
    movl $0, %eax       #eax == i
    movl $0, %ebx       #ebx == suma
    movl $Matriz, %ecx  #ecx == Matriz[0][0]

for:
    cmpl $10, %eax     #comparamos si i<10
    jge fi_for          #saltamos si i>=10
    
    addl 8(%ecx),%ebx    #suma+= Matriz[i][2]
    
    addl $40,%ecx       #pasamos a la siguiente fila
    incl %eax           #incrementamos i
    
    jmp for             #repetimos el for (nueva iteración)
fi_for:

#Ninst: 3 (preparacion de datos) + 6 * 10 (bucle) = 63


#----#1.3#----#
    movl $0, %eax       #i
    movl $0, %ebx       #j
    leal $ResFila, %ecx #ResFila (base)
    movl $1, (%ecx)     #ResFila[i] = 1
    leal $Matriz, %esi  #Matriz (base)
    
for:
    cmpl $10, $eax
    jge fi_for
    
while:
    #preparacion de Matriz[i][j] (acaba guardando en %edx)
    #@Matriz[i][j] = @Matriz[0][0]+(i*10+j)*4
    
    imull $10, %eax, %edx   #i*10
    addl %ebx, %edx         #i*10+j
    movl (%esi,%edx,4),%edx #base + (i*10 + j)*4
    
    cmpl $0, %edx
    je fi_while
    
if_while:
    cmpl $100, %edx
    jle fi_if_while
    subl $edx, $ecx
    
fi_if_while:
    incl %ebx
    jmp while
    
fi_while:
    addl $4, ecx       #pasamos a la next columna de ResFila
    movl $1, (%ecx)    #ResFila[i] = 1
    movl $0, %ebx
    incl %eax
    jmp for
    
fi_for:
  
