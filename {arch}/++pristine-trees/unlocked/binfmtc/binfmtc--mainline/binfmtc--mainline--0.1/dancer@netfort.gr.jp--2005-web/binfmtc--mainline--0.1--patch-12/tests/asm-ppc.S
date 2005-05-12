/*BINFMTASMCPP:
exit 1
	*/
	.file	"a.c"
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align 2
.LC1:
	.string	"Hello world %s\n"
	.align 2
.LC0:
	.string	"Hey!"
	.section	".text"
	.align 2
	.globl main
	.type	main, @function
main:
	stwu 1,-16(1)
	lis 3,.LC1@ha
	lis 4,.LC0@ha
	mflr 0
	la 3,.LC1@l(3)
	la 4,.LC0@l(4)
	stw 0,20(1)
	crxor 6,6,6
	bl printf
	lwz 0,20(1)
	li 3,0
	addi 1,1,16
	mtlr 0
	blr
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.3.5 (Debian 1:3.3.5-10)"
