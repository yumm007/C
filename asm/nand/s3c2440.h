#ifndef _S3C2440_H_
#define _S3C2440_H_

#define UART_CHANNELS	3
#define SPI_CHANNELS	2

#define ECCSIZE		512
#define ECCBYTES	3

enum uarts_nr {
	UART0,
	UART1,
	UART2
};

#define MEMCTL_BASE		0x48000000
#define USB_HOST_BASE		0x49000000
#define INTERRUPT_BASE		0x4A000000
#define DMA_BASE		0x4B000000
#define CLOCK_POWER_BASE	0x4C000000
#define LCD_BASE		0x4D000000
#define NAND_BASE		0x4E000000
#define UART_BASE		0x50000000
#define TIMER_BASE		0x51000000
#define USB_DEVICE_BASE		0x52000140
#define WATCHDOG_BASE		0x53000000
#define I2C_BASE		0x54000000
#define I2S_BASE		0x55000000
#define GPIO_BASE		0x56000000
#define RTC_BASE		0x57000000
#define ADC_BASE		0x58000000
#define SPI_BASE		0x59000000
#define SDI_BASE		0x5A000000


#include "regstruct.h"


static inline struct memctl *get_base_memctl(void)
{
	return (struct memctl *)MEMCTL_BASE;
}
static inline struct usb_host *get_base_usb_host(void)
{
	return (struct usb_host *)USB_HOST_BASE;
}
static inline struct interrupt *get_base_interrupt(void)
{
	return (struct interrupt *)INTERRUPT_BASE;
}
static inline struct dmas *get_base_dmas(void)
{
	return (struct dmas *)DMA_BASE;
}
static inline struct clock_power *get_base_clock_power(void)
{
	return (struct clock_power *)CLOCK_POWER_BASE;
}
static inline struct lcd *get_base_lcd(void)
{
	return (struct lcd *)LCD_BASE;
}
static inline struct nand *get_base_nand(void)
{
	return (struct nand *)NAND_BASE;
}
static inline struct uart *get_base_uart(enum uarts_nr n)
{
	return (struct uart *)(UART_BASE + (n * 0x4000));
}
static inline struct timers *get_base_timers(void)
{
	return (struct timers *)TIMER_BASE;
}
static inline struct usb_device *get_base_usb_device(void)
{
	return (struct usb_device *)USB_DEVICE_BASE;
}
static inline struct watchdog *get_base_watchdog(void)
{
	return (struct watchdog *)WATCHDOG_BASE;
}
static inline struct i2c *get_base_i2c(void)
{
	return (struct i2c *)I2C_BASE;
}
static inline struct i2s *get_base_i2s(void)
{
	return (struct i2s *)I2S_BASE;
}
static inline struct gpio *get_base_gpio(void)
{
	return (struct gpio *)GPIO_BASE;
}
static inline struct rtc *get_base_rtc(void)
{
	return (struct rtc *)RTC_BASE;
}
static inline struct adc *get_base_adc(void)
{
	return (struct adc *)ADC_BASE;
}
static inline struct spi *get_base_spi(void)
{
	return (struct spi *)SPI_BASE;
}
static inline struct sdi *get_base_sdi(void)
{
	return (struct sdi *)SDI_BASE;
}

#endif
