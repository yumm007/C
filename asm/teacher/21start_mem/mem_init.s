	.text
	.align 2
.globl mem_init
mem_init:
	ldr     r0, =REGDATA
	ldr	r1, =0x33f80000
	sub	r0, r0, r1
	ldr	r1, =0x48000000		@MEMCTL_BASE
	add     r2, r0, #13*4
0:
	ldr     r3, [r0], #4
	str     r3, [r1], #4
	cmp     r2, r0
	bne     0b

	mov	pc, lr

REGDATA:
	.word   0x2211d110	@BWSCON
	.word   0x00000700	@BACKCON0
	.word   0x00000700	@BACKCON1
	.word   0x00000700	@BACKCON2
	.word   0x00000700	@BACKCON3
	.word   0x00000700	@BACKCON4
	.word   0x00000700	@BACKCON5
	.word   0x00018005	@BACKCON6
	.word   0x00018005	@BACKCON7
	.word   0x00ac04f5	@REFRESH
	.word   0x000000b1	@BANKSIZE
	.word   0x00000030	@MRSRB6
	.word	0x00000030	@MRSRB7

