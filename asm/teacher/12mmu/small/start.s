
.global _start
_start:
	b	start_code
	nop
	nop
	ldr	pc, _prefetch
	ldr	pc, _data
	nop
	nop
	nop
_prefetch:
	.word prefetch
_data:
	.word data

start_code:
	stmfd	sp!, {lr}	

	mov	r0, #0
	ldr	r1, =0x30000000
0:
	ldr	r3, [r1], #4
	str	r3, [r0], #4
	cmp	r0, #4 * 16
	blt	0b

	bl	main

	ldmfd	sp!, {pc}

prefetch:
	mov	sp, #0x31000000
	stmfd	sp!, {r0}

	mov	r1, r0

	ldr	r0, =str1
	mov	lr, pc
	ldr	pc, printf
	
	ldmfd	sp!, {pc}^
	
data:
	mov	sp, #0x31000000
	stmfd	sp!, {lr}

	ldr	r0, =str2
	mov	lr, pc
	ldr	pc, printf

	ldmfd	sp!, {lr}
	subs	pc, lr, #8

printf:
	.word 0x33f9291c
str1:
	.string	"prefetch abort....., %08x\n"
str2:
	.string	"data abort.....\n"

