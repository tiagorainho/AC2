 .equ PRINT_STR,8
 .equ PRINT_CHAR, 3
 .equ GET_CHAR, 2
 .data
msg: .asciiz "Done\n"
 .text
 .globl main
main:

do: 
    li $v0, GET_CHAR
    syscall
    move $t0, $v0

    move $a0, $t0
    li $v0, PRINT_CHAR
    syscall

    bne $t0, '\n', do
    
la $a0, msg
li $v0, PRINT_STR
syscall


 li $v0,0 # return 0;
 jr $ra
