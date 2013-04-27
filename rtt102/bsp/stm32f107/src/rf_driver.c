
#include "rf_io.h"
#include "rf_driver.h"
#include "stm32f10x.h"

void rf_gpio_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin   = A7106_SCS_PIN | A7106_SCK_PIN | A7106_SDIO_PIN | GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(A7106_SCS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = A7106_GIO1_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(A7106_GIO1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = A7700_TXSW_PIN | A7700_RXSW_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(A7700_TXSW_PORT, &GPIO_InitStructure);

	CS_HIGH();
	SCK_HIGH();
	SDIO_OUTPUT();
	SDIO_HIGH();
	PA_SLEEP();
}

static void rf_sendbyte(UINT8 data){
	int i;

	for(i = 0; i < 8; i++)
	{
		if(data & 0x80)
		{
			SDIO_HIGH();
		}
		else
		{
			SDIO_LOW();
		}

		SCK_HIGH();
		data = data << 1;
		__NOP();
		SCK_LOW();
	}
}

static UINT8 rf_readbyte(void){
	int i;
	UINT8 tmp = 0;

	SDIO_INPUT();

	for(i = 0; i < 8; i++)
	{
		if(SDIO_READ())
		{
			tmp = (tmp << 1) | 0x01;
		}
		else
		{
			tmp = tmp << 1;
		}
	
		SCK_HIGH();
		__NOP();
		__NOP();
		SCK_LOW();	
	}

	SDIO_OUTPUT();
	return tmp;
}

void rf_cmd(UINT8 cmd)
{
	CS_LOW();
	rf_sendbyte(cmd);
	CS_HIGH();
}

void rf_writereg(UINT8 regaddr, UINT8 databyte){
	CS_LOW();
	rf_sendbyte(regaddr);
	rf_sendbyte(databyte);
	CS_HIGH();	
}

UINT8 rf_readreg(UINT8 regaddr)
{
	UINT8 ret;

	regaddr |= 0x40;

	CS_LOW();
	rf_sendbyte(regaddr);
	ret = rf_readbyte();
	CS_HIGH();

	return ret;
}

void rf_setfifolen(UINT8 len){
	rf_writereg(FIFO1_REG, len - 1);
}

UINT8 rf_readfifo(UINT8* dst, UINT8 len)
{
	UINT8 i;
	
	CS_LOW();
	rf_sendbyte(FIFO_REG | 0x40);
	for(i = 0; i < len; i++)
	{
		dst[i] = rf_readbyte();
	}	
	CS_HIGH();
	
	rf_cmd(CMD_RFR);
	return i;	
}


UINT8 rf_writefifo(const UINT8* src, UINT8 len)
{
	UINT8 i;

	rf_setfifolen(len);
	rf_cmd(CMD_TFR);

	CS_LOW();
	rf_sendbyte(FIFO_REG);
	for(i = 0; i < len; i++)
	{
		rf_sendbyte(src[i]);
	}
	CS_HIGH();

	return i;
}

void rf_writeid(const UINT8* id_buf)
{
	int i;

	CS_LOW(); // cs = 0

	rf_sendbyte(IDCODE_REG);

	for(i = 0; i < 4; i++)
	{
		rf_sendbyte(id_buf[i]);
	}

	CS_HIGH();  // cs = 1
}

void rf_set_ch(UINT8 ch) {
	rf_writereg(PLL1_REG, ch);
}

void rf_setpamode(pa_mode_t mode)
{
	switch(mode)
	{
		case PA_MODE_RX:
			PA_RX();
			break;
		case PA_MODE_TX:
			PA_TX();
			break;
		case PA_MODE_SLEEP:
			PA_SLEEP();
			break;
		default:
			PA_SLEEP();
			break;
	}
}

BOOL rf_issending(void)
{	
	return GIO1_READ() ? TRUE : FALSE;
}

BOOL RF_CRCCheck(void){
	return (rf_readreg(MODE_REG) & 0x20) == 0 ? TRUE : FALSE;
}

void RF_Delay100us(UINT16 num)
{
	UINT16 i, j;

	for(i = 0; i < num; i++)
	{
		for(j = 0; j < 1025; j++)
		{
		}
	}
}

BOOL RF_WaitForTransforComplete(UINT16 timeout)
{
	UINT16 i = timeout;
	BOOL ret = TRUE;

	if(i == 0xffff)
	{
		while(rf_issending());
	}
	else
	{
		while(rf_issending())
		{
			RF_Delay100us(1);
			
			if(i == 0)
			{
				ret = FALSE;
				break;
			}
			i--;
		}	
	}
	
	return ret;
}


typedef enum
{
	transfor_type_idle = 0,
	transfor_type_tx,
	transfor_type_rx,
}transfor_type_t;

#define USE_PA

void RF_TransforCmd(transfor_type_t type)
{
	if(type == transfor_type_tx)
	{
#ifdef USE_PA
		rf_setpamode(PA_MODE_TX);
#endif	
		rf_cmd(CMD_TX);
	}
	else if(type == transfor_type_rx)
	{
#ifdef USE_PA
		rf_setpamode(PA_MODE_RX);
#endif	
		rf_cmd(CMD_RX);
	}
	else
	{
#ifdef USE_PA
		rf_setpamode(PA_MODE_SLEEP);
#endif	
		rf_cmd(CMD_PLL);
	}
}

static const UINT8 ap_rf_add[] = AP_RF_ADD;

//发送定长的数据给终端
UINT8 rf_send(const UINT8 *id, const UINT8 *data, UINT8 len) {
	UINT8 ret;
	
	rf_writeid(id);
	rf_writefifo(data, len);

	RF_TransforCmd(transfor_type_tx);

	ret = RF_WaitForTransforComplete(RF_SND_TM) ? 0: 1;

	RF_TransforCmd(transfor_type_idle);
	return ret;	
}

//接收定长的数据到buf
UINT8 rf_recv(UINT8 *buf, UINT8 len) {
	UINT8 ret = 0;

	rf_writeid(ap_rf_add);
	rf_setfifolen(len);

	RF_TransforCmd(transfor_type_rx);
	if(!RF_WaitForTransforComplete(RF_RCV_TM)) {
		ret = 2;
		goto __exit;
	}

	if(!RF_CRCCheck()){
		ret = 1;
		goto __exit;
	}

	rf_readfifo(buf, len);

__exit:
	RF_TransforCmd(transfor_type_idle);

	return ret;	
}


