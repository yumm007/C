

.global _start
_start:
	nop
	nop
	ldr	pc, =swi
	nop
	nop
	nop
	nop
	nop

swi:
	stmfd	sp!, {lr}

	ldr	r0, [lr, #-4]
	bic	r0, r0, #0xff000000
	bl	syscall

	ldmfd	sp!, {pc}^ 

printf:
	.word 0x33f9291c

