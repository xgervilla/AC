.text
   .align 4
   .globl Insertar
   .type Insertar,@function
Insertar:
#tamElem = 12bytes
#v[i] = @v[0]+i*tamElem = i * 12 + 8(%ebp)
#i: -12(%ebp)
#j: -8(%ebp)
#lug: -4(%ebp)
#x.c = 12(%ebp); x.k = 16(%ebp); 20(%ebp
/* EL PROBLEMA ESTÁ ANTES DEL FOR: mal cálculo de lug (creo) */
   pushl %ebp
   movl %esp, %ebp
   subl $12, %esp      #espacio para i,j,lug
   
   movl $0, -12(%ebp)  #i = 0
   movl $-1, -4(%ebp)  #lug = -1
   
   imull $12, -12(%ebp), %ecx  #i*12
   movl 12(%ebp, %ecx), %ecx   #v[i].k
do:
   cmpl 16(%ebp), %ecx  #v[i].k - x.k
   jge primer_else       #salta si v[i].k <= x.k 
#if:
   movl -12(%ebp), %edx    #i
   movl %edx, -4(%ebp)     #lug = i
   jmp fi_do_while        #break 

primer_else:
   incl -12(%ebp)       #i++
#while:
    imull $12, -12(%ebp), %eax  #(i+1)*12
    movl 12(%ebp, %eax), %ecx   #v[i+1].k
    cmpl $0x80000000, %ecx
    jne do         #continua si v[i+1].k == 0x8000000
   
fi_do_while:
   #arribem si v[i].k == 0x80000000 || x.k < v[i].k
   cmpl $0x80000000, %ecx
   jne segon_else
#if:
   movl -12(%ebp), %edx     #i
   movl %edx, -4(%ebp)      #lug = i
   jmp fi_else    #saltem l'else
   
segon_else:
   imull $12, -12(%ebp), %eax  #(i)*12
   movl 12(%ebp, %eax), %ecx   #v[i].k
#while:
   cmpl $0x80000000, %ecx
   je fi_else     #salta si v[i].k == 0x8000000
   incl -12(%ebp) #i++
   jmp segon_else #repetim el bucle
/* EL FOR LO HACE CORRECTAMENTE ASÍ QUE EL PROBLEMA ES ANTERIOR */
fi_else:
   #aquí arribem quan v[i].k == 0x80000000
   
   movl -12(%ebp), %ecx     #j = i
   #per evitar tants accesos a memòria en comptes d'utilitzar -8(%ebp) per cada accés a j utilitzarem el registre %ecx
for:
   cmpl -4(%ebp), %ecx     #j-lug
   jg fi_for
#interior del for
#v[j].c = j*12 + 8(%ebp)    v[j+1].c = j*12+20(%ebp)
#v[j].k = j*12 + 12(%ebp)   v[j+1].c = j*12+24(%ebp)
#v[j].m = j*12 + 16(%ebp)   v[j+1].c = j*12+28(%ebp)

   imull $12, %ecx, %eax       #j*12
   
   movb 8(%ebp, %eax), %dl     #v[j].c
   movb %dl, 20(%ebp, %eax)    #v[j+1].c = v[j].c
   
   movl 12(%ebp, %eax), %edx   #v[j].k
   movl %edx, 24(%ebp, %eax)   #v[j+1].k = v[j].k
   
   movl 16(%ebp, %eax), %edx   #v[j].m
   movl %edx, 28(%ebp, %eax)   #v[j+1].m = v[j].m
   
#preparació de la nova iteració
   decl %ecx   #j--
   jmp for
   
fi_for:
   #preparem la pila (pas de paràmetres) i saltem
   pushl -4(%ebp)  #lug
   pushl 20(%ebp)  #x.m
   pushl 16(%ebp)  #x.k
   pushl 12(%ebp)  #x.c
   pushl 8(%ebp)   #@v
   
   call Asignar
   addl $20, %esp
   
   movl $1, %eax
   addl -12(%ebp), %eax     #i+1
   
   
   movl %ebp, %esp
   popl %ebp
   ret
