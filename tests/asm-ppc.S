/*BINFMTASMCPP:
exit 1
*/
	.section	.rodata
	.align 2
.LC0:
	.string	"hello\n"
	.section	".text"
	.align 2
	.p2align 4,,15
	.globl main
	.type	main, @function
main:
	lis 4,.LC0@ha
	li 3,1
	la 4,.LC0@l(4)
	li 5,6
	li 0,4
	sc
	li 3,0
	blr
	.size	main,.-main
