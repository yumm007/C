	.file	"count.c"
	.text
	.type	count, @function
count:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	%eax, %edx
	andl	$1431655765, %edx
	movl	8(%ebp), %eax
	andl	$-1431655766, %eax
	shrl	%eax
	leal	(%edx,%eax), %eax
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, %edx
	andl	$858993459, %edx
	movl	8(%ebp), %eax
	andl	$-858993460, %eax
	shrl	$2, %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, %edx
	andl	$252645135, %edx
	movl	8(%ebp), %eax
	andl	$-252645136, %eax
	shrl	$4, %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, %edx
	andl	$16711935, %edx
	movl	8(%ebp), %eax
	andl	$-16711936, %eax
	shrl	$8, %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	movzwl	%ax,%edx
	movl	8(%ebp), %eax
	movw	$0, %ax
	shrl	$16, %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	popl	%ebp
	ret
	.size	count, .-count
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
	subl	$36, %esp
	movl	$0, -8(%ebp)
	jmp	.L4
.L5:
	movl	-8(%ebp), %eax
	movl	%eax, (%esp)
	call	count
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	addl	$1, -8(%ebp)
.L4:
	cmpl	$255, -8(%ebp)
	jle	.L5
	movl	$0, %eax
	addl	$36, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-48)"
	.section	.note.GNU-stack,"",@progbits
