.data
str: .string

a: .long 0

i: .long 0

arr: .long 0, 0, 0, 0, 0

F0: .string "THIS IS FUNCTION WITH 0 ARGS!\n"
v: .long 0

F1: .string "THIS IS FUNCTION WITH 1 ARG: %d\n"
var1: .long 0

var2: .long 0

F2: .string "THIS IS FUNCTION WITH 2 ARGS: "
F3: .string "%d AND %d\n"
F4: .string "GIVE ME YOUR NUM: "
F5: .string "%d"
F6: .string "HERE IS YOUR NUM: %d\n"
F7: .string "GIVE ME YOUR STRING: "
F8: .string "%s"
F9: .string "HERE IS YOUR STRING: %s\n"
pr1: .long 0
pr2: .long 0
F10: .string "TESTING ADD: 2+2 = %d\n"
F11: .string "TESTING SUB: 2-4 = %d\n"
F12: .string "TESTING MUL: 2*2 = %d\n"
F13: .string "TESTING DIV: 11 div 5 = %d\n"
F14: .string "TESTING MOD: 11 div 5 = %d\n"
F15: .string "TESTING POLISH NOTATION: ((2+1)*(3+7)) div 10 = %d\n"
F16: .string "TESTING INT VAR ASSIGN: a = %d\n"
F17: .string "TESTING ARRAY: %d%d%d%d%d\n"
F18: .string "TESTING ARRAY ARITHMETICS: %d\n"
F19: .string "a = %d\n"
F20: .string "TESTING IF/ELSE CONSTRUCTION: "
F21: .string "VAR A IS BIGGER THAN 100\n"
F22: .string "VAR A IS EQUAL 100\n"
F23: .string "VAR A LESS THAN 100\n"
F24: .string "VAR ARR[4] IS BIGGER THAN 100\n"
F25: .string "VAR ARR[4] IS EQUAL 100\n"
F26: .string "VAR ARR[4] LESS THAN 100\n"
F27: .string "i = %d\n"
F28: .string "TESTING REPEAT/UNTIL CONSTRUCTION"
F29: .string " VAR A NOW %d"
F30: .string " BUT AFTER REPUN IT BECAME %d\n"
F31: .string "TESTING WHILE CONSTRUCTION"
F32: .string " VAR A NOW %d"
F33: .string " BUT AFTER WHILE IT BECAME %d\n"
F34: .string "TESTING LOOP WITH ARRAY\n"
F35: .string "arr[%d] = %d\n"
F36: .string "TESTING FUNCTIONS\n"
test0:
pushl $F0
call printf
addl  $4, %esp
ret
test1:
movl %eax, v
pushl v
pushl $F1
call printf
addl  $8, %esp
ret
test2:
movl %eax, var1
movl %ebx, var2
pushl $F2
call printf
addl  $4, %esp
pushl var2
pushl var1
pushl $F3
call printf
addl  $12, %esp
ret
.text
.globl main
main:
pushl $F4
call printf
addl  $4, %esp
pushl $a
pushl $F5
call scanf
addl  $8, %esp
pushl a
pushl $F6
call printf
addl  $8, %esp
pushl $F7
call printf
addl  $4, %esp
pushl $str
pushl $F8
call scanf
addl  $8, %esp
pushl $str
pushl $F9
call printf
addl  $8, %esp
movl $2, %eax
addl $2, %eax
movl %eax, pr1
pushl pr1
pushl $F10
call printf
addl  $8, %esp
movl $2, %eax
subl $4, %eax
movl %eax, pr1
pushl pr1
pushl $F11
call printf
addl  $8, %esp
movl $2, %eax
movl $2, %ebx
imull %ebx
movl %eax, pr1
pushl pr1
pushl $F12
call printf
addl  $8, %esp
movl $0, %edx
movl $11, %eax
movl $0, %edx
movl $5, %ebx
idivl %ebx
movl %eax, pr1
pushl pr1
pushl $F13
call printf
addl  $8, %esp
movl $0, %edx
movl $11, %eax
movl $0, %edx
movl $5, %ebx
idivl %ebx
movl %edx, pr1
pushl pr1
pushl $F14
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
pushl $F15
call printf
addl  $8, %esp
movl $0, a
pushl a
pushl $F16
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
pushl $F17
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
pushl $F18
call printf
addl  $8, %esp
movl a, %eax
addl $4, %eax
movl %eax, pr1
movl %eax , a
pushl a
pushl $F19
call printf
addl  $8, %esp
pushl $F20
call printf
addl  $4, %esp
movl $100, %eax
cmpl %eax, a
jg B0_0
jmp C0_1
jmp BE0
B0_0:
pushl $F21
call printf
addl  $4, %esp
jmp BE0
C0_1:
movl $100, %eax
cmpl %eax, a
je B0_1
jmp C0_2
jmp BE0
B0_1:
pushl $F22
call printf
addl  $4, %esp
jmp BE0
C0_2:
pushl $F23
call printf
addl  $4, %esp
movl $100, %eax
movl $4, %ecx
cmpl %eax, arr(, %ecx, 4)
jg B1_0
jmp C1_1
jmp BE1
B1_0:
pushl $F24
call printf
addl  $4, %esp
jmp BE1
C1_1:
movl $100, %eax
movl $4, %ecx
cmpl %eax, arr(, %ecx, 4)
je B1_1
jmp C1_2
jmp BE1
B1_1:
pushl $F25
call printf
addl  $4, %esp
jmp BE1
C1_2:
pushl $F26
call printf
addl  $4, %esp
BE1:
BE0:
movl $5, %eax
addl $5, %eax
movl %eax, pr1
movl %eax , i
jmp L0_check
L0:
pushl i
pushl $F27
call printf
addl  $8, %esp
decl i
L0_check:
movl $4, %ecx
movl arr(, %ecx, 4), %eax
cmpl %eax, i
jnl L0
pushl $F28
call printf
addl  $4, %esp
pushl a
pushl $F29
call printf
addl  $8, %esp
L1:
movl a, %eax
addl $1, %eax
movl %eax, pr1
movl %eax , a
movl $50, %eax
cmpl %eax, a
jl L1
pushl a
pushl $F30
call printf
addl  $8, %esp
pushl $F31
call printf
addl  $4, %esp
pushl a
pushl $F32
call printf
addl  $8, %esp
jmp L2_check
L2:
movl a, %eax
subl $1, %eax
movl %eax, pr1
movl %eax , a
L2_check:
movl $10, %eax
cmpl %eax, a
jg L2
pushl a
pushl $F33
call printf
addl  $8, %esp
pushl $F34
call printf
addl  $4, %esp
movl $0, i
jmp L3_check
L3:
movl i, %ecx
pushl arr(, %ecx, 4)
pushl i
pushl $F35
call printf
addl  $12, %esp
movl i, %eax
addl $1, %eax
movl %eax, pr1
movl %eax , i
L3_check:
movl $5, %eax
cmpl %eax, i
jl L3
pushl $F36
call printf
addl  $4, %esp
call test0
movl $2, %eax
call test1
movl i, %eax
movl $1, %ecx
movl arr(, %ecx, 4), %ebx
call test2
ret
