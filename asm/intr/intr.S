.global _start
_start:
	b init_timer
	nop
	nop
	nop
	nop
	nop
	ldr pc, timer
	nop
timer:
	.word timer_fun
	
init_timer:
	stmfd sp!, {lr} @ 使用Uboot自己的栈
	mov r0, #0
	ldr r1, =_start

cp:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, #4*9
	blt cp
	
	bl main	@打开中断响应	

	ldmfd sp!, {pc}

timer_fun:
	mov sp, #0x33000000
	stmfd sp!, {lr}
	bl do_irq
	ldmfd sp!, {pc}^  @从此模式中断返回时, movs pc, r14

