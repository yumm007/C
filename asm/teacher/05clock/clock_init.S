/*
*	#define MDIV_200	0x5c
*	#define PDIV_200	0x1
*	#define SDIV_200	0x2	
*/

#define MDIV_405 	0x7f
#define PDIV_405	0x2
#define SDIV_405	0x1

#define	CLKDIVN		0x4C000014
#define MPLLCON		0x4C000004


.global _start
_start:
	ldr	r0, =CLKDIVN
	mov	r1, #0x5	
	str	r1, [r0]

	/*
	* HDIVN = 0b01; PDIVN = 0b1; FCLK:HCLK:PCLK = 1:2:4
	* HDIVN = 0b10; PDIVN = 0b1;
	* 如果clock->CAMDIVN[9] == 0, FCLK:HCLK:PCLK = 1:4:8
	* 如果clock->CAMDIVN[9] == 1, FCLK:HCLK:PCLK = 1:8:16
	*
	* 如果HDIVN非0，cpu的总线模式应该从快速总线模式(fast bus mode)
	* 变为异步总线模式(asynchronous bus mode) 
	*
	* 如果不改变,arm核的频率将自动变为HCLK,而不再是FCLK
	*/

	mrc	p15, 0, r1, c1, c0, 0
	orr	r1, r1, #0xc0000000
	mcr	p15, 0, r1, c1, c0, 0


	ldr	r0, =MPLLCON
	ldr	r1, =(MDIV_405 << 12) | (PDIV_405 << 4) | SDIV_405
	str	r1, [r0]

	/*
	* FCLK = (2 * m * Fin) / (p * (2 ^ s));	// 2 ^ s表示2的s次方
	* m = MDIV_405 + 8 = 0x7f + 8 = 135;
	* p = PDIV_405 + 2 = 0x2 + 2  = 4;
	* s = SDIV_405 = 1;
	* Fin = 12M;
	*
	* FCLK = (2 * 135 * 12) / (4 * (2 ^ 1)) = 3240 / 8 = 405M;
	*/

	mov	pc, lr


