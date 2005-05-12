/*BINFMTASMCPP:
exit 1
	*/
.LC0:
        .string "Hello world"
        .text
        .p2align 4,,15
.globl main
        .type   main, @function
main:
        pushl   %ebp
        movl    %esp, %ebp
        subl    $8, %esp
        andl    $-16, %esp
        movl    $.LC0, (%esp)
        call    puts
        movl    %ebp, %esp
        xorl    %eax, %eax
        popl    %ebp
        ret
        .size   main, .-main
