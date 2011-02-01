
#define GPECON 0x56000040
#define GPEDAT 0x56000044

.global _start
_start:
	ldr	r0, =GPECON
	ldr	r1, [r0]
	bic	r1, r1, #0x0f000000
	orr	r1, r1, #0x05000000
	str	r1, [r0]


	ldr	r4, =GPEDAT
	ldr	r5, [r4]

loop:
	bic	r5, r5, #0x3000
	orr	r5, r5, #0x1000
	str	r5, [r4]

	mov	r0, #5
	bl	delay
	
	bic	r5, r5, #0x3000
	orr	r5, r5, #0x2000
	str	r5, [r4]

	mov	r0, #5
	bl	delay

	b	loop

	bx	lr

delay:
	ldr	r1, =10000000
	mul	r2, r0, r1
0:
	subs	r2, r2, #1
	bne	0b
	
	bx	lr

