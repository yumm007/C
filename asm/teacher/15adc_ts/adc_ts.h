
#define UD_SEN(x)	((x) << 8)
#define YM_SEN(x)	((x) << 7)  
#define YP_SEN(x) 	((x) << 6)
#define XM_SEN(x)	((x) << 5)
#define XP_SEN(x)	((x) << 4)
#define PULL_UP(x)	((x) << 3)
#define AUTO_PST(x)	((x) << 2)
#define XY_PST(x)	(x)

#define WAIT_DOWN	(UD_SEN(0) | YM_SEN(1) | YP_SEN(1) | XM_SEN(0) | \
			XP_SEN(1) | PULL_UP(0) | XY_PST(3))

#define WAIT_UP		(UD_SEN(1) | YM_SEN(1) | YP_SEN(1) | XM_SEN(0) | \
			XP_SEN(1) | PULL_UP(0) | XY_PST(3))

#define AUTO_XY		(PULL_UP(1) | AUTO_PST(1) | XY_PST(0))




