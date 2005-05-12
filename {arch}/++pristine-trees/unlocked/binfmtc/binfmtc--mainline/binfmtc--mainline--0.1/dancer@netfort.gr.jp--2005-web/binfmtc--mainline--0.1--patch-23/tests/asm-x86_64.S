/*BINFMTASMCPP:
exit 1
	*/
.HELLOWORLD:
	.string	"Hello world"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	movl	$.HELLOWORLD, %edi
	call	puts
	xorl	%eax, %eax
	.size	main, .-main
