#include "s3c2440.h"
#include "common.h"

#define 	BUF		0x31000000
#define	BUFSIZE	0xfffff

extern void dma_fun(void);
extern void wm8976_init(void);
extern void init_irq(void);

void init_i2s(void) {
	struct i2s *const i2s = get_base_i2s();

	setval(i2s->IISCON,0x1,1,1);
	setval(i2s->IISCON,0x1,1,5);

	setval(i2s->IISMOD,0x2,2,6);
	setval(i2s->IISMOD,0xd,4,0);

	setval(i2s->IISPSR,0x2,5,5); /* 50 * 1024 / 384 / 44.1 = 3, = IISPSR + 1 */

	setval(i2s->IISFCON,0x1,1,15); /* Transfer DMA mode */
	setval(i2s->IISFCON,0x1,1,13); /* Transfer FIFO mode */
}

void init_dma(void) {
	struct dma *const dma =((struct dma *)get_base_dmas()) + 0;
	struct i2s *const i2s = get_base_i2s();

   /* 设置源 */
   dma->DISRC = BUF;
   dma->DISRCC = 0; /* 数据源在内存中，ahb */

   /* 设置目的地址 */
   dma->DIDST =(unsigned int)&i2s->IISFIFO;
   dma->DIDSTC |= 3;  /* 目的地址uart连接在apb上*/

   setval(dma->DCON, 0x1, 1, 29); /* 启动中断通知 */
   setval(dma->DCON, 0x5, 3, 24); /* 接上IIS */
   setval(dma->DCON, 0x1, 1, 23); /*  硬件DMA*/
   setval(dma->DCON, 0x1, 1, 22); /* 关闭重载 */
   setval(dma->DCON, 0x01, 2, 20); /* 每次传输16个位 */
   setval(dma->DCON, BUFSIZE, 20, 0); 
}

int main(void)
{
	struct dma *const dma =((struct dma *)get_base_dmas()) + 0;
	struct i2s *const i2s = get_base_i2s();

	wm8976_init();
	init_irq();
	init_i2s();
	init_dma();

   /* 激活DMA  */
	i2s->IISCON |= 1;
   dma->DMASKTRIG |= 0x2;

	return 0;
}
