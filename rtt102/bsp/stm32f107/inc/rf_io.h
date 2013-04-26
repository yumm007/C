#ifndef _RF_IO_
#define _RF_IO_

#include "stm32f10x.h"

#define 	A7106_SCS_PORT			GPIOD
#define 	A7106_SCS_PIN			GPIO_Pin_15
#define 	A7106_SCK_PORT			GPIOD
#define 	A7106_SCK_PIN			GPIO_Pin_14
#define 	A7106_SDIO_PORT			GPIOD
#define 	A7106_SDIO_PIN			GPIO_Pin_13
#define 	A7106_GIO1_PORT	 		GPIOC
#define 	A7106_GIO1_PIN	 		GPIO_Pin_9
#define 	A7106_GIO2_PORT	 		GPIOC
#define 	A7106_GIO2_PIN	 		GPIO_Pin_8
#define 	A7700_TXSW_PORT	 		GPIOC
#define 	A7700_TXSW_PIN	  		GPIO_Pin_6
#define 	A7700_RXSW_PORT	 		GPIOC
#define 	A7700_RXSW_PIN	  		GPIO_Pin_7

__inline void CS_HIGH() // CS_HIGH
{
//	GPIO_SetBits(A7106_SCS_PORT, A7106_SCS_PIN);
	A7106_SCS_PORT->BSRR = A7106_SCS_PIN;	
}

__inline void CS_LOW() // CS_LOW
{
//	GPIO_ResetBits(A7106_SCS_PORT, A7106_SCS_PIN);
	A7106_SCS_PORT->BRR = A7106_SCS_PIN;
}

__inline void SCK_HIGH()	// SCK_HIGH
{
//	GPIO_SetBits(A7106_SCK_PORT, A7106_SCK_PIN);
	A7106_SCK_PORT->BSRR = A7106_SCK_PIN;	
}

__inline void SCK_LOW()	// SCK_LOW
{
//	GPIO_ResetBits(A7106_SCK_PORT, A7106_SCK_PIN);
	A7106_SCK_PORT->BRR = A7106_SCK_PIN;
}

__inline void SDIO_HIGH()	  //SDIO_HIGH
{
//	GPIO_SetBits(A7106_SDIO_PORT, A7106_SDIO_PIN);
	A7106_SDIO_PORT->BSRR = A7106_SDIO_PIN;	
}

__inline void SDIO_LOW()	   //SDIO_LOW
{
//	GPIO_ResetBits(A7106_SDIO_PORT, A7106_SDIO_PIN);
	A7106_SDIO_PORT->BRR = A7106_SDIO_PIN;
}

__inline unsigned char SDIO_READ()
{
	return GPIO_ReadInputDataBit(A7106_SDIO_PORT, A7106_SDIO_PIN);
}

__inline void SDIO_INPUT()
{
	GPIO_InitTypeDef sdio;
	
	sdio.GPIO_Pin   = A7106_SDIO_PIN; 
	sdio.GPIO_Speed = GPIO_Speed_50MHz;
	sdio.GPIO_Mode  = GPIO_Mode_IN_FLOATING;

	GPIO_Init(A7106_SDIO_PORT, &sdio);
}

__inline void SDIO_OUTPUT()
{
	GPIO_InitTypeDef sdio;
	
	sdio.GPIO_Pin   = A7106_SDIO_PIN; 
	sdio.GPIO_Speed = GPIO_Speed_50MHz;
	sdio.GPIO_Mode  = GPIO_Mode_Out_PP;

	GPIO_Init(A7106_SDIO_PORT, &sdio);

}

__inline void PA_TX(void)	 //PA_TX
{
//	GPIO_ResetBits(A7700_TXSW_PORT, A7700_TXSW_PIN);
	A7700_TXSW_PORT->BRR = A7700_TXSW_PIN;
//	GPIO_SetBits(A7700_RXSW_PORT, A7700_RXSW_PIN);
	A7700_RXSW_PORT->BSRR = A7700_RXSW_PIN;
}

__inline void PA_RX()  // PA_RX
{
//	GPIO_SetBits(A7700_TXSW_PORT, A7700_TXSW_PIN);
	A7700_TXSW_PORT->BSRR = A7700_TXSW_PIN;
//	GPIO_ResetBits(A7700_RXSW_PORT, A7700_RXSW_PIN);
	A7700_RXSW_PORT->BRR = A7700_RXSW_PIN;
}

__inline void PA_SLEEP()  //PA_SLEEP
{
//	GPIO_SetBits(A7700_TXSW_PORT, A7700_TXSW_PIN);
	A7700_TXSW_PORT->BSRR = A7700_TXSW_PIN;
//	GPIO_SetBits(A7700_RXSW_PORT, A7700_RXSW_PIN); // txsw = 1, rxsw = 1 a7700 sleep
	A7700_RXSW_PORT->BSRR = A7700_RXSW_PIN;
}

__inline unsigned char GIO1_READ()
{
	return GPIO_ReadInputDataBit(A7106_GIO1_PORT, A7106_GIO1_PIN);
}


#endif

