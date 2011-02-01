#ifndef	_TEST_H
#define _TEST_H
#include <linux/ioctl.h>
struct my_t{
	int header;	
	char buf[20];
};

#define		MAGIC_LED	'x'
#define		LED_ON		_IO(MAGIC_LED, 0)	
#define		LED_OFF		_IO(MAGIC_LED, 1)

#define		MAGIC_RTC	'z'
#define		RTC_START	_IO(MAGIC_RTC, 0)

#endif	/* _TEST_H */
