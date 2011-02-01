	.arch armv4t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"com_init.c"
	.text
	.align	2
	.type	set_txd, %function
set_txd:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	mov	r1, #1442840576
	add	r1, r1, #112
	mov	r3, #1442840576
	add	r3, r3, #112
	ldr	r0, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	mov	r2, r3, asl #2
	mov	r3, #15
	mov	r3, r3, asl r2
	mvn	r3, r3
	and	r3, r0, r3
	str	r3, [r1, #0]
	mov	r1, #1442840576
	add	r1, r1, #112
	mov	r3, #1442840576
	add	r3, r3, #112
	ldr	r0, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	mov	r2, r3, asl #2
	mov	r3, #10
	mov	r3, r3, asl r2
	mvn	r3, r3
	orr	r3, r0, r3
	str	r3, [r1, #0]
	mov	r1, #1442840576
	add	r1, r1, #120
	mov	r3, #1442840576
	add	r3, r3, #120
	ldr	r0, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r2, r3, #2
	mov	r3, #3
	mov	r3, r3, asl r2
	orr	r3, r0, r3
	str	r3, [r1, #0]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	set_txd, .-set_txd
	.align	2
	.type	set_parity, %function
set_parity:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	mov	r0, r3
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	ldr	r3, [r3, #0]
	bic	r3, r3, #56
	str	r3, [r0, #0]
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	mov	r0, r3
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-12]
	mov	r3, r3, asl #3
	orr	r3, r2, r3
	str	r3, [r0, #0]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	set_parity, .-set_parity
	.align	2
	.type	set_stop, %function
set_stop:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	mov	r0, r3
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	ldr	r3, [r3, #0]
	bic	r3, r3, #4
	str	r3, [r0, #0]
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	mov	r0, r3
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-12]
	mov	r3, r3, asl #2
	mvn	r3, r3
	orr	r3, r2, r3
	str	r3, [r0, #0]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	set_stop, .-set_stop
	.align	2
	.type	set_data_bit, %function
set_data_bit:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	mov	r0, r3
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	ldr	r3, [r3, #0]
	bic	r3, r3, #3
	str	r3, [r0, #0]
	ldr	r1, [fp, #-8]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	ldr	r3, [r3, #0]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	set_data_bit, .-set_data_bit
	.align	2
	.type	__aeabi_uidiv, %function
__aeabi_uidiv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	mov	r3, #0
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-20]
	cmp	r3, #0
	bne	.L10
.L11:
	b	.L11
.L10:
	ldr	r3, [fp, #-20]
	cmp	r3, #1
	bne	.L14
	ldr	r3, [fp, #-16]
	str	r3, [fp, #-24]
	b	.L13
.L15:
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-20]
	rsb	r3, r3, r2
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L14:
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-20]
	cmp	r2, r3
	bhi	.L15
	ldr	r3, [fp, #-8]
	str	r3, [fp, #-24]
.L13:
	ldr	r3, [fp, #-24]
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	__aeabi_uidiv, .-__aeabi_uidiv
	.global	__aeabi_uidiv
	.align	2
	.type	set_bund_rate, %function
set_bund_rate:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #12
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	ldr	r1, [fp, #-16]
	mov	r3, r1
	mov	r2, r3, asl #2
	mov	r3, r2, asl #5
	rsb	r3, r2, r3
	add	r3, r3, r1
	mov	r3, r3, asl #5
	add	r3, r3, #1342177280
	add	r3, r3, #40
	mov	r4, r3
	ldr	r3, [fp, #-20]
	mov	r2, r3, asl #4
	mov	r3, #49807360
	add	r3, r3, #192512
	add	r3, r3, #128
	mov	r0, r3
	mov	r1, r2
	bl	__aeabi_uidiv
	mov	r3, r0
	sub	r3, r3, #1
	str	r3, [r4, #0]
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, lr}
	bx	lr
	.size	set_bund_rate, .-set_bund_rate
	.align	2
	.global	init_com
	.type	init_com, %function
init_com:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #0]
	mov	r0, r3
	bl	set_txd
	ldr	r3, [fp, #-8]
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #4]
	mov	r0, r2
	mov	r1, r3
	bl	set_parity
	ldr	r3, [fp, #-8]
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #8]
	mov	r0, r2
	mov	r1, r3
	bl	set_stop
	ldr	r3, [fp, #-8]
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #12]
	mov	r0, r2
	mov	r1, r3
	bl	set_data_bit
	ldr	r3, [fp, #-8]
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #16]
	mov	r0, r2
	mov	r1, r3
	bl	set_bund_rate
	sub	sp, fp, #4
	ldmfd	sp!, {fp, lr}
	bx	lr
	.size	init_com, .-init_com
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits
