.data
arr: .long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

min: .long 0

i: .long 0

F0: .string "введите 10 элементов массива\n"
F1: .string "%d"
F2: .string "Элемент записан"
F3: .string ", вводите следующий\n"
pr1: .long 0
pr2: .long 0
F4: .string "Мин. элемент массива = %d\n"
.text
.globl main
main:
pushl $F0
call printf
addl  $4, %esp
movl $10000, min
movl $0, i
jmp L0_check
L0:
movl i, %ecx
movl $arr, %ebx
L1:
addl $4, %ebx
loop L1
pushl %ebx
pushl $F1
call scanf
addl  $8, %esp
pushl $F2
call printf
addl  $4, %esp
pushl $F3
call printf
addl  $4, %esp
movl i, %ecx
movl arr(, %ecx, 4), %eax
cmpl %eax, min
jg B0_0
jmp BE0
B0_0:
movl $i, %ecx
movl %eax, min
jmp BE0
BE0:
movl i, %eax
addl $1, %eax
movl %eax, pr1
movl %eax , i
L0_check:
movl $10, %eax
cmpl %eax, i
jl L0
pushl min
pushl $F4
call printf
addl  $8, %esp
ret
