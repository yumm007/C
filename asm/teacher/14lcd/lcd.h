/************************ CON1 ********************************/
#define ENVID(x)    	(x)
#define BPPMODE(x)   	((x) << 1)
#define PNRMODE(x)	((x) << 5)
#define MMODE(x)	((x) << 7)
#define CLKVAL(x)	((x) << 8)

/************************ CON2 ********************************/
#define VSPW(x)		(x)
#define VFPD(x)  	((x) << 6)
#define LINEVAL(x)	((x) << 14)
#define VBPD(x)		((x) << 24)

/************************ CON3 ********************************/
#define HFPD(x)		(x)
#define HOZVAL(x)	((x) << 8)
#define HBPD(x)		((x) << 19)

/************************ CON4 ********************************/
#define HSPW(x)		(x)

/************************ CON5 ********************************/
#define HWSWP(x)	(x)
#define BSWP(x)		((x) << 1)
#define ENLEND(x)	((x) << 2)
#define PWREN(x)	((x) << 3)
#define INVLEND(x)	((x) << 4)
#define INVPWREN(x)	((x) << 5)
#define INVVDEN(x)	((x) << 6)
#define INVVD(x)	((x) << 7)
#define INVVFRAME(x)	((x) << 8)
#define INVVLINE(x)	((x) << 9)
#define INVVCLK(x)	((x) << 10)
#define FRM565(x)	((x) << 11)
#define BPP24BL(x)	((x) << 12)

/*********************** ADDR1 *********************************/
#define LCDBASEU(x)	(x)
#define LCDBANK(x)	((x) << 21)

/*********************** ADDR2 *********************************/
#define LCDBASEL(x)	(x) 

/*********************** ADDR3 *********************************/
#define PAGEWIDTH(x)	(x)
#define OFFSIZE(x)	((x) << 11)



