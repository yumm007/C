
.global _start
_start:
	ldr     r0, =data
	ldr	r1, =0x48000000		@MEMCTL_BASE
	add     r2, r0, #13*4
0:
	ldr     r3, [r0], #4
	str     r3, [r1], #4
	cmp     r2, r0
	bne     0b

	mov	pc, lr


data:
	.word 	0x22111110
	.word	0x00000700
	.word	0x00000700
	.word	0x00000700
	.word	0x00000700
	.word	0x00000700
	.word	0x00000700
	.word	0x00018005
	.word	0x00018005
	.word	0x00ac04f6
	.word	0x000000b2
	.word	0x00000030
	.word	0x00000030



