#ifndef _BOOTM_H_
#define _BOOTM_H_

#define ATAG_NONE	0x00000000
struct tag_header {
	u32 size;
	u32 tag;
};

#define ATAG_CORE	0x54410001
struct tag_core {
	u32 flags;
	u32 pagesize;
	u32 rootdev;
};

#define ATAG_CMDLINE	0x54410009
struct tag_cmdline {
	char	cmdline[1];
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_cmdline	cmdline;
	} u;
};

#define tag_next(t)	((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

#endif

