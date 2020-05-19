.data
str: .string


F0: .string "GIVE ME YOUR STRING: "
F1: .string "%s"
F2: .string "HERE IS YOUR STRING: %s\n"
.text
.globl main
main:
pushl $F0
call printf
addl  $4, %esp
pushl $str
pushl $F1
call scanf
addl  $8, %esp
pushl $str
pushl $F2
call printf
addl  $8, %esp
ret
