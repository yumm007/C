

.global _start
_start:
	stmfd sp!, {lr}

	ldr r0, [pc, #16]
	mov lr, pc
	ldr pc, [pc, #4]
	mov r0, #0
	ldmfd sp!, {pc}

	.word 0x33f9291c
	.word str
str:
	.string "this is print line.\n"
