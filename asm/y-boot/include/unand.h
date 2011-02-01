typedef unsigned int volatile u32t;

extern void nand_init();
extern u32t nand_read(u32t,  u32t, int);
extern u32t nand_write(u32t, u32t, int);
extern int	nand_erase(u32t, int);
