#include "s3c2440.h"
#include "common.h"

char buf[] = "line from buf.\n\r";

void _start(void)
{
	struct uart *const uart = get_base_uart(0);
	struct dma *const dma =((struct dma *)get_base_dmas()) + 0;

	/*开启uart0 的DMA received 功能*/
	//uart->UCON |= 0x2 << 2;
	uart->UCON = 9;

	/* 设置源 */
	dma->DISRC = (unsigned int)buf;
	dma->DISRCC = 0; /* 数据源在内存中，ahb */

	/* 设置目的地址 */
	dma->DIDST =(unsigned int)&uart->UTXH;
	dma->DIDSTC |= 3;  /* 目的地址uart连接在apb上*/

	setval(dma->DCON, 0x001, 3, 24); /* 接上uart0  */
	setval(dma->DCON, 0x1, 1, 23); /*  硬件DMA*/
	setval(dma->DCON, 0x1, 1, 22); /* 关闭重载 */
	setval(dma->DCON, sizeof(buf), 20, 0);

	/* 激活DMA  */
	dma->DMASKTRIG |= 0x2;

	return;
}
