#include <common.h>
#include <command.h>
#include <s3c2440.h>
#include <interrupt.h>

void pwm_start(void)
{
	struct watchdog *const wdt = get_base_watchdog();

	 wdt->WTCNT = wdt->WTDAT;
         
	 printf("feed dog....\n");
}

int do_wdt(int argc, char *argv[])
{
	struct watchdog *const wdt = get_base_watchdog();
	struct timers *const pwm = get_base_timers();

	wdt->WTDAT = 62500;
	wdt->WTCNT = 62500;
	wdt->WTCON = (49 << 8) | (1 << 5) | 1;

	pwm->TCFG0 = 49 << 8;
	pwm->TCFG1 = 3 << 16;
	pwm->TCNTB4 = 50000;
	pwm->TCON = 5 << 20;

	request_irq(INT_TIMER4, pwm_start);


	pwm_start();

	return 0;
}


G_BOOT_CMD(wdt, do_wdt);
