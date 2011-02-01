.global _start
_start:
	b cp
	nop
	nop
	nop
	nop
	nop
	ldr pc, irq
	nop
irq:
	.word handler 

cp:
	stmfd sp!, {lr}
	ldr r0, =_start
	mov r1, #0

l:
	ldr r2, [r0], #4
	str r2, [r1], #4
	cmp r1, #4 * 9
	blt l

	bl main

	ldmfd sp!, {pc}

handler:
	mov sp, #0x33000000	
	stmfd sp!, {r1 - r12, lr}

	bl touch_screen
	
	ldmfd sp!, {r1 - r12, lr}
	subs pc, lr, #4

