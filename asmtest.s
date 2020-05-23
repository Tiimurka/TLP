.data
str: .string


a: .long 0


arr: .long 0, 0, 0, 0, 0


F0: .string "GIVE ME YOUR NUM: "
F1: .string "%d"
F2: .string "HERE IS YOUR NUM: %d\n"
pr1: .long 0
pr2: .long 0
F3: .string "TESTING ADD: 2+2 = %d\n"
F4: .string "TESTING SUB: 2-4 = %d\n"
F5: .string "TESTING MUL: 2*2 = %d\n"
F6: .string "TESTING DIV: 11 div 5 = %d\n"
F7: .string "TESTING MOD: 11 div 5 = %d\n"
F8: .string "TESTING POLISH NOTATION: ((2+1)*(3+7)) div 10 = %d\n"
F9: .string "TESTING INT VAR ASSIGN: a = %d\n"
F10: .string "TESTING ARRAY: %d%d%d%d%d\n"
F11: .string "TESTING ARRAY ARITHMETICS: %d\n"
F12: .string "a = %d\n"
.text
.globl main
main:
pushl $F0
call printf
addl  $4, %esp
pushl $a
pushl $F1
call scanf
addl  $8, %esp
pushl a
pushl $F2
call printf
addl  $8, %esp
movl $2, %eax
addl $2, %eax
movl %eax, pr1
pushl pr1
pushl $F3
call printf
addl  $8, %esp
movl $2, %eax
subl $4, %eax
movl %eax, pr1
pushl pr1
pushl $F4
call printf
addl  $8, %esp
movl $2, %eax
movl $2, %ebx
imull %ebx
movl %eax, pr1
pushl pr1
pushl $F5
call printf
addl  $8, %esp
movl $0, %edx
movl $11, %eax
movl $0, %edx
movl $5, %ebx
idivl %ebx
movl %eax, pr1
pushl pr1
pushl $F6
call printf
addl  $8, %esp
movl $0, %edx
movl $11, %eax
movl $0, %edx
movl $5, %ebx
idivl %ebx
movl %edx, pr1
pushl pr1
pushl $F7
call printf
addl  $8, %esp
movl $2, %eax
addl $1, %eax
movl %eax, pr1
movl $3, %eax
addl $7, %eax
movl %eax, pr2
movl pr1, %eax
movl pr2, %ebx
imull %ebx
movl %eax, pr1
movl $0, %edx
movl pr1, %eax
movl $0, %edx
movl $10, %ebx
idivl %ebx
movl %eax, pr1
pushl pr1
pushl $F8
call printf
addl  $8, %esp
movl $322, a
pushl a
pushl $F9
call printf
addl  $8, %esp
movl $0, %ecx
movl $1, arr(, %ecx, 4)
movl $1, %ecx
movl $2, arr(, %ecx, 4)
movl $2, %ecx
movl $3, arr(, %ecx, 4)
movl $3, %ecx
movl $4, arr(, %ecx, 4)
movl $4, %ecx
movl $5, arr(, %ecx, 4)
movl $4, %ecx
pushl arr(, %ecx, 4)
movl $3, %ecx
pushl arr(, %ecx, 4)
movl $2, %ecx
pushl arr(, %ecx, 4)
movl $1, %ecx
pushl arr(, %ecx, 4)
movl $0, %ecx
pushl arr(, %ecx, 4)
pushl $F10
call printf
addl  $24, %esp
movl $0, %ecx
movl arr(, %ecx, 4), %eax
movl $1, %ecx
addl arr(, %ecx, 4), %eax
movl %eax, pr1
movl pr1, %eax
movl $2, %ecx
addl arr(, %ecx, 4), %eax
movl %eax, pr1
movl pr1, %eax
movl $3, %ecx
addl arr(, %ecx, 4), %eax
movl %eax, pr1
movl pr1, %eax
movl $4, %ecx
addl arr(, %ecx, 4), %eax
movl %eax, pr1
movl pr1, %eax
addl a, %eax
movl %eax, pr1
pushl pr1
pushl $F11
call printf
addl  $8, %esp
movl a, %eax
addl $4, %eax
movl %eax, pr1
movl %eax , a
pushl a
pushl $F12
call printf
addl  $8, %esp
ret
