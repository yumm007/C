#ifndef	_LCD_H
#define	_LCD_H

#define	MAGIC_LCD	'x'
#define	CONFIG_LCD	_IO(MAGIC_LCD, 0)
#define	ENABLE_LCD	_IO(MAGIC_LCD, 1)
#define	DISABLE_LCD	_IO(MAGIC_LCD, 2)

#endif	/* _LCD_H */
