#include <s3c2440.h>

unsigned int get_fclk(void)
{
	unsigned int r, m, p, s;

	struct clock_power *const clock = get_base_clock_power();

	r = clock->MPLLCON;
	
	m = ((r & 0xff000) >> 12) + 8;
	p = ((r & 0x003f0) >> 4) + 2;
	s = r & 0x3;

	return (12 * m * 2) / (p << s) * 1000000;
}

unsigned int get_hclk(void)
{
	struct clock_power *const clock = get_base_clock_power();
	
	unsigned rc = 0;
	unsigned int cam = clock->CAMDIVN;
	
	unsigned int clk = (clock->CLKDIVN >> 1) & 0x3;
	
	unsigned int fclk = get_fclk();

	switch(clk)
	{
		case 0:
			rc = fclk;
			break;
		case 1:
			rc = fclk >> 1;			//除2
			break;
		case 2:
			if(!(cam & 0x200))
				rc = fclk >> 2;		//除4
			else
				rc = fclk >> 3;		//除8
			break;
		case 3:
			if(!(cam & 0x100))
				rc = fclk / 3;		//除3
			else
				rc = fclk / 6;		//除6
			break;		
	}

	return rc;
}

unsigned int get_pclk(void)
{
	struct clock_power *const clock = get_base_clock_power();
	if(!(clock->CLKDIVN & 0x1))
		return get_hclk();

	return get_hclk() >> 1;
}


