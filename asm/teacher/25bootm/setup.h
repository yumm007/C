#ifndef _SETUP_H_
#define _SETUP_H_

/* The list ends with an ATAG_NONE node. */
#define ATAG_NONE	0x00000000

struct tag_header {
	u32 size;
	u32 tag;
};

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE	0x54410001

struct tag_core {
	u32 flags;		/* bit 0 = read-only */
	u32 pagesize;
	u32 rootdev;
};


/* command line: \0 terminated string */
#define ATAG_CMDLINE	0x54410009

struct tag_cmdline {
	char	cmdline[1];	/* this is the minimum size */
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
