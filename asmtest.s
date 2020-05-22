.data
str: .string


a: .long 0


F0: .string "GIVE ME YOUR STRING: "
F1: .string "%s"
F2: .string "HERE IS YOUR STRING: %s\n"
pr1: .long 0
pr2: .long 0
F3: .string "TESTING ADD: 2+2 = %d\n"
F4: .string "TESTING SUB: 2-4 = %d\n"
F5: .string "TESTING MUL: 2*2 = %d\n"
F6: .string "TESTING DIV: 11 div 5 = %d\n"
F7: .string "TESTING MOD: 11 div 5 = %d\n"
F8: .string "TESTING POLISH NOTATION: ((2+1)*(3+7)) div 10 = %d\n"
F9: .string "TESTING INT VAR ASSIGN: a = %d\n"
F10: .string "zaz"
F11: .string "TESTING STRING VAR ASSIGN: str = %s\n"
.text
.globl main
main:
pushl $F0
call printf
addl  $4, %esp
pushl str
pushl $F1
call scanf
addl  $8, %esp
pushl str
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
movl $161, %eax
addl $161, %eax
movl %eax, pr1
movl %eax , a
pushl a
pushl $F9
call printf
addl  $8, %esp
movl $F10, str
pushl str
pushl $F11
call printf
addl  $8, %esp
ret
