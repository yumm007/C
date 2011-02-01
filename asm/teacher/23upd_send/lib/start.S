	.text
.globl _start
_start:
	ldr	pc, _start_code
	ldr	pc, _und
	ldr	pc, _swi
	ldr	pc, _prefetch
	ldr	pc, _data
	b	_start
	ldr	pc, _irq
	ldr	pc, _fiq

_start_code:
	.word	start_code
_und:
	.word	und
_swi:
	.word	swi
_prefetch:
	.word	prefetch
_data:
	.word	data
_irq:
	.word	irq
_fiq:
	.word	fiq


start_code:
	stmfd	sp!, {r4 - r12, lr}

	mov	r4, #0x30000000
	mov	r5, #0
	mov	r6, #4*15
0:
	ldr	r7, [r4], #4
	str	r7, [r5], #4
	cmp	r5, r6
	blt	0b

	ldr 	r4, bss_start
	ldr 	r5, bss_end
	mov 	r6, #0
1:
	str 	r6, [r4], #4
	cmp 	r4, r5
	blt 	1b

	stmfd	sp!, {r0 - r3}

	bl	serial_init
	bl	irq_init

	ldmfd	sp!, {r0 - r3}

	bl	main

	ldmfd	sp!, {r4 - r12, pc}
	
bss_start:
	.word __bss_start
bss_end:
	.word __bss_end

