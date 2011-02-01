
.global _start
_start:
	b	main
	nop
	ldr	pc, _swi
	nop
	nop
	nop
	nop
	nop
_swi:
	.word swi

main:
	stmfd	sp!, {lr}	

	mov	r0, #0
	ldr	r1, =0x30000000
0:
	ldr	r3, [r1], #4
	str	r3, [r0], #4
	cmp	r0, #4 * 9
	blt	0b

	swi	#3

	ldmfd	sp!, {pc}

swi:
	stmfd	sp!, {lr}

	ldr	r0, [lr, #-4]
	bic	r0, r0, #0xff000000
	bl	syscall

	ldmfd	sp!, {pc}^ 

