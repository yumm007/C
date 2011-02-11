	.file	"test1.c"
	.text
	.type	func_b, @function
func_b:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	addl	$6, %eax
	popl	%ebp
	ret
	.size	func_b, .-func_b
	.type	func_a, @function
func_a:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	pushl	%eax
	call	func_b
	addl	12(%ebp), %eax
	leave
	ret
	.size	func_a, .-func_a
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$20, %esp
	movl	$6, 4(%esp)
	movl	$5, (%esp)
	call	func_a
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	addl	$20, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-48)"
	.section	.note.GNU-stack,"",@progbits
