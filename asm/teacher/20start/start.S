

.global _start
_start:
	ldr	r0, =0x53000000
	mov	r1, #0
	str	r1, [r0]
	
	mov	sp, #0x1000
	
	bl	clock_init

	bl	main
0:
	b	0b

