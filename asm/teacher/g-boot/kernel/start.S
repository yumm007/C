	.text
.globl _start
_start:
	b	start_code
	ldr 	pc, _undefined_instruction
	ldr 	pc, _software_interrupt
	ldr 	pc, _prefetch_abort
	ldr 	pc, _data_abort
	b	start_code
	ldr 	pc, _irq
	ldr 	pc, _fiq

_undefined_instruction:
	.word undefined_instruction
_software_interrupt: 
	.word software_interrupt
_prefetch_abort: 
	.word prefetch_abort
_data_abort: 
	.word data_abort
_irq: 
	.word irq
_fiq: 
	.word fiq

start_code:
	mov	r0, #0xd3
	msr	cpsr, r0
	
	ldr	r0, =0x53000000
	mov	r1, #0
	str	r1, [r0]

	bl	clock_init

	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0

	bl	mem_init

	mov 	sp, #0x33800000
	mov	fp, #0

	ldr	r0, =0x33f80000
	mov	r1, #0
	ldr	r2, bss_start
	sub	r2, r2, r0
	bl	nand_read_lowlevel
	tst	r0, #0
	beq	clear_bss
1:
	b	1b

clear_bss:
	ldr 	r0, bss_start
	ldr 	r1, bss_end
	mov 	r2, #0
2:
	str 	r2,[r0],#4
	cmp 	r0, r1
	blt 	2b

/* 从cpu的内部sram跳到sdram上执行 */
	ldr	pc, =run_on_sdram	
run_on_sdram:
	
	bl	console_init
	bl	irq_init
	bl	nand_init
	bl	shell
3:
	b	3b

bss_start:
	.word __bss_start
bss_end:
	.word __bss_end

