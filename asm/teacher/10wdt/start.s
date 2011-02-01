
.global _start
_start:
	b	start_code
	nop
	nop
	nop
	nop
	nop
	ldr	pc, _irq
	nop
_irq:
	.word irq

start_code:
	stmfd	sp!, {lr}	

	mov	r0, #0
	ldr	r1, =0x30000000
0:
	ldr	r3, [r1], #4
	str	r3, [r0], #4
	cmp	r0, #4 * 9
	blt	0b

	bl	main

	ldmfd	sp!, {pc}

irq:
	mov	sp, #0x33000000
	stmfd	sp!, {r0 - r12, lr}

	bl	do_irq

	ldmfd	sp!, {r0 - r12, lr}
	subs	pc, lr, #4

