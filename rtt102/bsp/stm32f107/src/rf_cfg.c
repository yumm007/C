
#include "rf_cfg.h"
#include "rf_driver.h"

static BOOL rf_cal(void);
static BOOL rf_rco_cal(void);
static void rf_config(void);

//更新参数设置，验证WAKEUPID, CRC错误
//张冬波修改2012-11-6 21:05:02
#if 1
//static UINT8 rf_config_array[] = 
UINT8 rf_config_array[] = 
{
    //      address   name              Descript
    //      -------   ----              ---------
    0x00,   //0x00  ; MODE_REG
    0x62,   //0x01  ; MODE_CONTROL_REG  FIFO mode, Enable ARSSI, Enable AIF
    0x00,   //0x02  ; CALIBRATION_REG
    0x3F,   //0x03  ; FIFO1_REG         FIFO end = 63+1
    0x40,   //0x04  ; FIFO2_REG
    0x00,   //0x05  ; FIFO_REG
    0x00,   //0x06  ; IDCODE_REG
    0x00,   //0x07  ; RCOSC1_REG
    0x00,   //0x08  ; RCOSC2_REG
    0x00,   //0x09  ; RCOSC3_REG
    0x00,   //0x0A  ; CKO_REG
	
    0x01, // 0x0B   GPIO1 register          WTR
    0x01, // 0x0C   GPIO2 register,         WTR    0x01, // 0x0B   GPIO1 register          WTR
//  0x19, // 0x0C   GPIO2 register,         SDO    0x19, // 0x0B   GPIO1 register          SDO       WTR
    0x05,   //0x0D  ; CLOCK_REG
	//Data Rate
    0x00,   //0x0E  ; DATARATE_REG      500K bps
    0x64,   //0x0F  ; PLL1_REG
    0x9E,   //0x10  ; PLL2_REG
    0x4B,   //0x11  ; PLL3_REG
    0x00,   //0x12  ; PLL4_REG
    0x02,   //0x13  ; PLL5_REG
    0x16,   //0x14  ; TX1_REG           f(dev) = 186kHz
    0x2B,   //0x15  ; TX2_REG
    0x12,   //0x16  ; DELAY1_REG        TX, PLL setting time = 60us
    0x40,   //0x17  ; DELAY2_REG        Crystal Turn on delay = 600us
    0x62,   //0x18  ; RX_REG            BW = 500kHz, Up side band
    0x80,   //0x19  ; RXGAIN1
    0x80,   //0x1A  ; RXGAIN2
    0x00,   //0x1B  ; RXGAIN3
    0x0A,   //0x1C  ; RXGAIN4
    0x32,   //0x1D  ; RSSI_REG
    0x03,   //0x1E  ; ADC_REG
	
//    0x0C,   //0x1F  ; CODE1_REG//ID=4bytes, Preamble=1bytes, enable CRC
    0x0F,   //0x1F  ; CODE1_REG//ID=4bytes, Preamble=4bytes, enable CRC	
    0x12,   //0x20  ; CODE2_REG
    0x00,   //0x21  ; CODE3_REG
    0x00,   //0x22  ; IFCAL1_REG
    0x00,   //0x23  ; IFCAL2_REG
    0x00,   //0x24  ; VCOCCAL_REG       VCO current auto calibration
    0x00,   //0x25  ; VCOCAL1_REG
    0x23,   //0x26  ; VCOCAL2_REG
    0x70,   //0x27  ; BATTERY_REG       Regulator = 1.8V, Enable Quick discharge
    //TX POWER
	//0x17, //0x28  ; TXTEST_REG        TX power = 0dBm
	0x37,   //0x28  ; TXTEST_REG        TX power = 1dBm
    0x47,   //0x29  ; RXDEM1_REG
    0x80,   //0x2A  ; RXDEM2_REG
 // 0x57,   //0x2B  ; CPC_REG
 	0x77,   //0x2B
    0x01,   //0x2C  ; CRYSTALTEST_REG
    0x45,   //0x2D  ; PLLTEST_REG
    0x19,   //0x2E  ; VCOTEST1_REG
    0x00,   //0x2F  ; VCOTEST2_REG
    0x01,   //0x30  ; IFAT_REG
    0x0F,   //0x31  ; RSCALE_REG
    0x00,   //0x32  ; FILTERTEST_REG
    0x7F,   //0x33  ; TMV_REG           Ramp Up / Ramp Down enable
};
#else
//static UINT8 rf_config_array[] = 
UINT8 rf_config_array[] = 
{
    //      address   name              Descript
    //      -------   ----              ---------
    0x00,   //0x00  ; MODE_REG
    0x62,   //0x01  ; MODE_CONTROL_REG  FIFO mode, Enable ARSSI, Enable AIF
    0x00,   //0x02  ; CALIBRATION_REG
    0x3F,   //0x03  ; FIFO1_REG         FIFO end = 63+1
    0x40,   //0x04  ; FIFO2_REG
    0x00,   //0x05  ; FIFO_REG
    0x00,   //0x06  ; IDCODE_REG
    0xFF,   //0x07  ; RCOSC1_REG
    0xC7,   //0x08  ; RCOSC2_REG
    0x00,   //0x09  ; RCOSC3_REG
    0x00,   //0x0A  ; CKO_REG
	
    0x01, // 0x0B   GPIO1 register          WTR
    0x01, // 0x0C   GPIO2 register,         WTR    0x01, // 0x0B   GPIO1 register          WTR
//  0x19, // 0x0C   GPIO2 register,         SDO    0x19, // 0x0B   GPIO1 register          SDO       WTR
    0x05,   //0x0D  ; CLOCK_REG
	//Data Rate
    0x00,   //0x0E  ; DATARATE_REG      500K bps
    0x64,   //0x0F  ; PLL1_REG
    0x9E,   //0x10  ; PLL2_REG
    0x4B,   //0x11  ; PLL3_REG
    0x00,   //0x12  ; PLL4_REG
    0x02,   //0x13  ; PLL5_REG
    0x16,   //0x14  ; TX1_REG           f(dev) = 186kHz
    0x2B,   //0x15  ; TX2_REG
    0x12,   //0x16  ; DELAY1_REG        TX, PLL setting time = 60us
    0x40,   //0x17  ; DELAY2_REG        Crystal Turn on delay = 600us
    0x62,   //0x18  ; RX_REG            BW = 500kHz, Up side band
    0x80,   //0x19  ; RXGAIN1
    0x80,   //0x1A  ; RXGAIN2
    0x00,   //0x1B  ; RXGAIN3
    0x0A,   //0x1C  ; RXGAIN4
    0x32,   //0x1D  ; RSSI_REG
    0xC3,   //0x1E  ; ADC_REG
	
//    0x0C,   //0x1F  ; CODE1_REG//ID=4bytes, Preamble=1bytes, enable CRC
    0x0F,   //0x1F  ; CODE1_REG//ID=4bytes, Preamble=4bytes, enable CRC	
    0x12,   //0x20  ; CODE2_REG
    0x00,   //0x21  ; CODE3_REG
    0x00,   //0x22  ; IFCAL1_REG
    0x00,   //0x23  ; IFCAL2_REG
    0x0F,   //0x24  ; VCOCCAL_REG       VCO current auto calibration
    0x00,   //0x25  ; VCOCAL1_REG
    0x23,   //0x26  ; VCOCAL2_REG
    0x70,   //0x27  ; BATTERY_REG       Regulator = 1.8V, Enable Quick discharge
    //TX POWER
	//0x17, //0x28  ; TXTEST_REG        TX power = 0dBm
	0x37,   //0x28  ; TXTEST_REG        TX power = 1dBm
    0x47,   //0x29  ; RXDEM1_REG
    0x80,   //0x2A  ; RXDEM2_REG
 // 0x57,   //0x2B  ; CPC_REG
 	0x77,   //0x2B
    0x01,   //0x2C  ; CRYSTALTEST_REG
    0x45,   //0x2D  ; PLLTEST_REG
    0x19,   //0x2E  ; VCOTEST1_REG
    0x00,   //0x2F  ; VCOTEST2_REG
    0x01,   //0x30  ; IFAT_REG
    0x0F,   //0x31  ; RSCALE_REG
    0x00,   //0x32  ; FILTERTEST_REG
    0x7F,   //0x33  ; TMV_REG           Ramp Up / Ramp Down enable
};
#endif

BOOL bs_rf_init()
{
	BOOL ret = FALSE;
	UINT32 i, j;

	rf_gpio_init();
	//rf_timer_init();

	rf_writereg(MODE_REG, 0x00);
	rf_writereg(CPC_REG, 0x77);
	rf_config();	
	rf_writereg(CPC_REG,0x57);

	for(j = 0; j < 5; j++)
	{
		if(rf_cal() && rf_rco_cal())
		{
			rf_cmd(CMD_STBY);
			for(i = 0; i < 7777777; i++);
			ret = TRUE;
			break;	
		}
	}
	
	return ret;
}

void bs_rf_deinit()
{
	int i;
	rf_gpio_init();
	rf_cmd(CMD_IDLE);
	for(i = 0; i < 7777777; i++);
}

static BOOL rf_cal()
{
	UINT8 tmp0, tmp1, tmp2, tmp3;
	BOOL ret = FALSE;

	//==================================================
	// calibration IF, VB, VCB, RSSI
	rf_cmd(CMD_STBY);
	rf_writereg(CALIBRATION_REG, 0x0F);
	do
	{
		tmp0 = rf_readreg(CALIBRATION_REG);
	}while (tmp0 & 0x0F);

	tmp1 = rf_readreg(IFCAL1_REG);
	tmp2 = rf_readreg(VCOCCAL_REG);
	tmp3 = rf_readreg(VCOCAL1_REG);
	
	if (((tmp1 & 0x10) || (tmp2 & 0x10) || (tmp3 & 0x08)) == 0)
	{
		 ret = TRUE;
	}
	
	return ret;
}


static BOOL rf_rco_cal()
{
	UINT8 tmp, rcoc, rcot;
	BOOL ret = FALSE;
	//==================================================
	// calibration RC osc
	for(rcot = 0; rcot <= 3; rcot++)
	{
		rf_writereg(RCOSC3_REG, 0x00); // RCO oscillator disable
		rf_writereg(RCOSC3_REG, (0x0C | (rcot<<4))); // RCOSC_E=1, CALW=1
		do
		{
			tmp = rf_readreg(RCOSC1_REG) & 0x80;
		} while(tmp);

		rcoc = rf_readreg(RCOSC1_REG) & 0x3F;
		if ( rcoc >= 20 )
		{
			ret = TRUE;
			break;
		}
	}
	return ret;
}

static void rf_config()
{
	int i;

	//0x00 mode register, for reset
    //0x02 calibration register
    //0x05 fifo data register
    //0x06 id code register
    //0x23 IF calibration II, only read
    //0x32 filter test register

	for(i = 1; i <= 0x33; i++)
    {
        if ((i==0x02) || (i==0x05) || (i==0x06) || (i==0x23) || (i==0x32))
		{
			continue;
		}
        rf_writereg(i, rf_config_array[i]);
    }
}




