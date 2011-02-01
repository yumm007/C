.global _start
_start:
	b main
	nop
	ldr pc, swi
	nop
	nop
	nop
	nop
	nop
swi:
	.word handler @这里是一个地址

main:
	stmfd sp!, {lr}
	ldr r0, =_start
	mov r1, #0
l:
	ldr r2, [r0], #4
	str r2, [r1], #4
	cmp r1, #4 * 9
	blt l

	swi #4

	ldmfd sp!, {pc}
	@mov pc, lr

handler:
	mov r0, #0xAA
	movs pc, lr
