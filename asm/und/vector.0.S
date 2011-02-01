

.global _start
_start:
	nop
	ldr	pc, =und
	nop
	nop
	nop
	nop
	nop
	nop

und:
	mov	sp, #0x32000000
	stmfd	sp!, {lr}

	ldr	r0, =str
	
	ldr	r1, [lr, #-4]

	mrs	r2, cpsr

	mov	lr, pc
	ldr	pc, printf

	ldmfd	sp!, {pc}^ 


printf:
	.word 0x33f9291c
str:
	.string "hello und...., %08x, cpsr = %08x\n"



