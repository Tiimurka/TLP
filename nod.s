.data
k: .long 0

l: .long 0

n: .long 0

check: .long 0

F0: .string "Введите два числа: \n"
F1: .string "%d"
F2: .string "%d"
pr1: .long 0
pr2: .long 0
F3: .string "%d %d\n"
F4: .string "НОД = %d\n"
.text
.globl main
main:
pushl $F0
call printf
addl  $4, %esp
pushl $l
pushl $F1
call scanf
addl  $8, %esp
pushl $k
pushl $F2
call scanf
addl  $8, %esp
movl $2, check
L0:
movl l, %eax
cmpl %eax, k
jg B0_0
jmp C0_1
jmp BE0
B0_0:
movl $0, %edx
movl k, %eax
movl $0, %edx
movl l, %ebx
idivl %ebx
movl %edx, pr1
movl %edx , k
jmp BE0
C0_1:
movl $0, %edx
movl l, %eax
movl $0, %edx
movl k, %ebx
idivl %ebx
movl %edx, pr1
movl %edx , l
BE0:
pushl k
pushl l
pushl $F3
call printf
addl  $12, %esp
movl $0, %eax
cmpl %eax, k
je B1_0
jmp BE1
B1_0:
movl $0, check
jmp BE1
BE1:
movl $0, %eax
cmpl %eax, l
je B2_0
jmp BE2
B2_0:
movl $0, check
jmp BE2
BE2:
movl $0, %eax
cmpl %eax, check
jne L0
movl k, %eax
addl l, %eax
movl %eax, pr1
movl %eax , n
pushl n
pushl $F4
call printf
addl  $8, %esp
ret
