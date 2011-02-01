#ifndef	_WDT_H
#define	_WDT_H

#define	MAGIC_WDT	'x'
#define	WDT_FEED	_IO(MAGIC_WDT, 0)
#define	WDT_START	_IO(MAGIC_WDT, 1)
#define	WDT_STOP	_IO(MAGIC_WDT, 2)
#define	WDT_MODE	_IO(MAGIC_WDT, 3)

#endif	/* _WDT_H */
