
.global und
und:
	mov	sp, #0x33c00000
	mov	r0, #1
	b	do_interrupt

.global swi
swi:
	mov	r0, #2
	ldr	r1, [lr, #-4]
	bic	r1, r1, #0xff000000
	b	do_interrupt

.global prefetch
prefetch:
	mov	sp, #0x33c00000
	mov	r0, #3
	b	do_interrupt

.global data
data:
	mov	sp, #0x33c00000
	mov	r0, #4
	b	do_interrupt

.global irq
irq:
	mov	sp, #0x33c00000
	stmfd	sp!, {r0-r12, lr}
	bl	do_irq
	ldmfd	sp!, {r0-r12, lr}
	subs	pc, lr, #4

.global fiq
fiq:
	mov	sp, #0x33c00000
	mov	r0, #6
	b	do_interrupt

