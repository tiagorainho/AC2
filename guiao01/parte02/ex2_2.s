 .equ PRINT_STR,8
 .equ PRINT_CHAR, 3
 .equ IN_KETS, 1
 .data
msg: .asciiz "Done\n"
 .text
 .globl main
main:

do: 
    li $v0, IN_KEYS
    syscall
    move $t0, $v0

    beq $t0, $0, do

    move $a0, $t0
    li $v0, PRINT_CHAR
    syscall

    bne $t0, '\n', do
    
la $a0, msg
li $v0, PRINT_STR
syscall


 li $v0,0 # return 0;
 jr $ra
