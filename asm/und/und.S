.arch armv5te

.align 2
.global _start
_start:
	b	main
	ldr pc, fun
	ldr pc, fun
	ldr pc, fun
	ldr pc, fun
	ldr pc, fun
	ldr pc, fun
	ldr pc, fun
fun:
	.word handler

main:
	stmfd sp!, {lr}
	ldr r1, =_start
	mov r2, #0

loop:
	ldr r0, [r1], #4
	str r0, [r2], #4
	cmp r2, #4 * 9
	blt loop
	
	clz r0, r1

	mov r0, #0xAA
	ldmfd sp!, {pc}

handler:
	mov sp, #0x33000000
	stmfd sp!, {lr}
	ldr   r0, =str
	mov lr, pc
	ldr   pc, print
	ldmfd sp!, {pc}^
	
str:
	.string "I am hear.\n"
print:
	.word 0x33f9291c

