	.arch	armv5te

.global _start
_start:
	stmfd	sp!, {lr}

	ldr	r0, =str
	mrs	r1, cpsr
	mov	lr, pc
	ldr	pc, =0x33f9291c

	blx	r3

	ldr	r0, =str
	mrs	r1, cpsr
	mov	lr, pc
	ldr	pc, =0x33f9291c

	ldmfd	sp!, {pc}

str:
	.string "cpsr = %08x\n"

