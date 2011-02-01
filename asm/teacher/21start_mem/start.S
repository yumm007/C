	.text
.globl _start
_start:
	b	start_code
	nop
	nop
	nop
	nop
	nop
	nop
	nop

start_code:
	mov	r0, #0xd3
	msr	cpsr, r0
	
	ldr	r0, =0x53000000
	mov	r1, #0
	str	r1, [r0]

	bl	clock_init

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
	
	bl	serial_init
	bl	main
3:
	b	3b

bss_start:
	.word __bss_start
bss_end:
	.word __bss_end

