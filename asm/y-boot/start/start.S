#define	WATCHCLOCK	0x53000000
#define  CLKDIVN		0x4C000014
#define	MPLLCON		0x4C000004
#define	MEM_CTL_BASE 0x48000000

	.text
	.global _start
_start:
@中断地址
	b 		start_code @reset
.if 0
	ldr	pc, _undefined_instruction @未知指令
	ldr	pc, _software_interrupt		@软中断
	ldr	pc, _prefetch_abort			@取址错误
	ldr	pc, _data_about				@取数据错
	b		start_code						@未定义
	ldr	pc, _irq							@中断
	ldr	pc, _fiq							@快速中断

_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word software_interrupt
_prefetch_abort:
	.word prefetch_abort
_data_about:
	.word data_about
_irq:
	.word irq
_fiq:
	.word fiq
.endif

start_code:
@进入管理模式
	nop
	mov r0, #0xd3
	msr cpsr, r0

@关闭看门狗
	ldr r0, =WATCHCLOCK
	mov r1, #0
	str r1, [r0]

@初始化时钟
	ldr r0, =CLKDIVN @设置1：4：8
	mov r1, #0x5
	str r1, [r0]

	mrc p15, 0, r1, c1, c0, 0 @设置成异步总线模式
	orr r1, r1, #0xc0000000
	mcr p15, 0, r1, c1, c0, 0

	ldr r0, =MPLLCON	@设置FCLK
	ldr r1, = (127 << 12) | (2 << 4) | 1
	str r1, [r0]

@初始化CPU
   mov   r0, #0
   mcr   p15, 0, r0, c7, c7, 0   /* flush v3/v4 cache */
   mcr   p15, 0, r0, c8, c7, 0   /* flush v4 TLB */

   mrc   p15, 0, r0, c1, c0, 0
   bic   r0, r0, #0x00002300  @ clear bits 13, 9:8 (--V- --RS)
   bic   r0, r0, #0x00000087  @ clear bits 7, 2:0 (B--- -CAM)
   orr   r0, r0, #0x00000002  @ set bit 2 (A) Align
   orr   r0, r0, #0x00001000  @ set bit 12 (I) I-Cache
   mcr   p15, 0, r0, c1, c0, 0

@初始化内存
	ldr r1, =reg_val
	ldr r0, =0x33f80000
	sub r1, r1, r0 @r1的值现在还在4k空间内，，但是此时reg_val等于高地址，所有要减回来
	ldr r0, =MEM_CTL_BASE
	add r2, r1, #13*4
l:
	ldr r3, [r1], #4
	str r3, [r0], #4
	cmp r1, r2
	bne	l

@初始化栈，准备进入C代码
	mov sp, #0x33800000
	mov fp, #0	

@调用copy函数。将boot loader代码copy到高地址,r0为目标地址, r1源地址, r2长度
	bl nand_init
	ldr r0, =0x33f80000
	mov r1, #0
	ldr r2, _bss_start @程序链接时地址设置为0x33f80000，减去bss段的偏移量，即为代码段长度
	sub r2, r2, r0
	bl nand_read

@BSS段抹0
	ldr r0, _bss_start @bss_start地址值默认是在高地址,抹的是高地址
	ldr r1, _bss_end
	mov r2, #0
l1:
	str r2, [r0], #4
	cmp r0, r1
	blt l1

@只要确保以上代码以及它所调用的函数总共在4k以内就没问题
@当前pc值还在低4k范围内，但run_on_sdram的值在链接时已经指定到高地址
	ldr pc, =run_on_sdram 
run_on_sdram:				 

	bl gpio_init
	bl init_uart0
	@登记中断

@进入shell
	bl ushell

reg_val:
   .word   0x2211d110   @BWSCON                               
   .word   0x00000700   @BACKCON0                             
   .word   0x00000700   @BACKCON1
   .word   0x00000700   @BACKCON2                             
   .word   0x00000700   @BACKCON3                             
   .word   0x00000700   @BACKCON4                             
   .word   0x00000700   @BACKCON5                             
   .word   0x00018005   @BACKCON6                             
   .word   0x00018005   @BACKCON7                             
   .word   0x00ac04f5   @REFRESH                              
   .word   0x000000b1   @BANKSIZE                             
   .word   0x00000030   @MRSRB6                               
   .word   0x00000030	@MRSRB7

_bss_start:
	.word bss_start
_bss_end:
	.word bss_end
