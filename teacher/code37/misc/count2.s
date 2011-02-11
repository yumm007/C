	.file	"count2.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	xorl	%ebx, %ebx
	pushl	%ecx
	subl	$16, %esp
	.p2align 4,,7
.L2:
	movzbl	%bl, %eax
	movzbl	%bh, %ecx
	movl	data(,%eax,4), %edx
	movl	%ebx, %eax
	shrl	$16, %eax
	movzbl	%al, %eax
	addl	data(,%ecx,4), %edx
	addl	data(,%eax,4), %edx
	movl	%ebx, %eax
	shrl	$24, %eax
	addl	$1, %ebx
	addl	data(,%eax,4), %edx
	movl	$.LC0, (%esp)
	movl	%edx, 4(%esp)
	call	printf
	cmpl	$256, %ebx
	jne	.L2
	addl	$16, %esp
	xorl	%eax, %eax
	popl	%ecx
	popl	%ebx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.section	.rodata
	.align 32
	.type	data, @object
	.size	data, 1024
data:
	.long	0
	.long	1
	.long	1
	.long	2
	.long	1
	.long	2
	.long	2
	.long	3
	.long	1
	.long	2
	.long	2
	.long	3
	.long	2
	.long	3
	.long	3
	.long	4
	.long	1
	.long	2
	.long	2
	.long	3
	.long	2
	.long	3
	.long	3
	.long	4
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	1
	.long	2
	.long	2
	.long	3
	.long	2
	.long	3
	.long	3
	.long	4
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	1
	.long	2
	.long	2
	.long	3
	.long	2
	.long	3
	.long	3
	.long	4
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	4
	.long	5
	.long	5
	.long	6
	.long	5
	.long	6
	.long	6
	.long	7
	.long	1
	.long	2
	.long	2
	.long	3
	.long	2
	.long	3
	.long	3
	.long	4
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	4
	.long	5
	.long	5
	.long	6
	.long	5
	.long	6
	.long	6
	.long	7
	.long	2
	.long	3
	.long	3
	.long	4
	.long	3
	.long	4
	.long	4
	.long	5
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	4
	.long	5
	.long	5
	.long	6
	.long	5
	.long	6
	.long	6
	.long	7
	.long	3
	.long	4
	.long	4
	.long	5
	.long	4
	.long	5
	.long	5
	.long	6
	.long	4
	.long	5
	.long	5
	.long	6
	.long	5
	.long	6
	.long	6
	.long	7
	.long	4
	.long	5
	.long	5
	.long	6
	.long	5
	.long	6
	.long	6
	.long	7
	.long	5
	.long	6
	.long	6
	.long	7
	.long	6
	.long	7
	.long	7
	.long	8
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-48)"
	.section	.note.GNU-stack,"",@progbits
