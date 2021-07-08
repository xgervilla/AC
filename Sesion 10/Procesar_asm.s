#Xavier Gervilla Machado i Carlos Exojo Gavilan, grup 23
.text
    .align 4
    .globl procesar
    .type    procesar, @function
procesar:
    pushl    %ebp
    movl    %esp, %ebp
    subl    $16, %esp
    pushl    %ebx
    pushl    %esi
    pushl    %edi

# Aqui has de introducir el codigo
    movl $0, %esi         #contador
    movl 8(%ebp), %eax    #mata
    movl 12(%ebp), %ebx   #matb
    movl 16(%ebp), %ecx   #n
    imul %ecx, %ecx           #un solo bucle n*n

for:
    cmp %ecx, %esi   #comparamos nuestro contador (esi) con n*n
    jge fifor

    movb (%eax,%esi), %dl      #cargamos mata
    andb $1, %dl               #and con 1
    movb %dl, (%ebx,%esi)      #si es 0 ya se guarda (en caso de ser mayor sobreescribimos
    cmpb $0, %dl
    jle inc        #si es 0 saltamos este paso
    movb $255, (%ebx,%esi)
inc:
    incl %esi       #incrementamos nuestro contador y repetimos el bucle
    jmp for
fifor:
# El final de la rutina ya esta programado

    popl    %edi
    popl    %esi
    popl    %ebx
    movl %ebp,%esp
    popl %ebp
    ret
