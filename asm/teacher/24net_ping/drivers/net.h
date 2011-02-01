#ifndef _NET_H_
#define _NET_H_

#define htonl(d) swapl(d)
#define ntohl(d) swapl(d)
#define htons(d) swaps(d)
#define ntohs(d) swaps(d)

extern unsigned char ethaddr[];
extern unsigned int ipaddr;
extern unsigned int serverip;

struct ethhdr{
	unsigned char	dest[6];	/* destination eth addr	*/
	unsigned char	source[6];	/* source ether addr	*/
	unsigned short	proto;		/* packet type ID field	*/
} __attribute__((packed));

typedef struct ethhdr eth_t;

struct iphdr{
    unsigned int 	ihl:4;
    unsigned int 	version:4;
    unsigned char 	tos;
    unsigned short 	tot_len;
    unsigned short 	id;
    unsigned short 	frag_off;
    unsigned char 	ttl;
    unsigned char 	protocol;
    unsigned short 	check;
    unsigned int 	saddr;
    unsigned int 	daddr;
} __attribute__((packed));

typedef struct iphdr ip_t;

struct udphdr{
  unsigned short source;
  unsigned short dest;
  unsigned short len;
  unsigned short check;
} __attribute__((packed));

typedef struct udphdr udp_t;

struct arphdr
{
	unsigned short	ar_hrd;		/* format of hardware address	*/
	unsigned short	ar_pro;		/* format of protocol address	*/
	unsigned char	ar_hln;		/* length of hardware address	*/
	unsigned char	ar_pln;		/* length of protocol address	*/
	unsigned short	ar_op;		/* ARP opcode (command)		*/
	unsigned char	ar_sha[6];	/* sender hardware address	*/
	unsigned int 	ar_sip;		/* sender IP address		*/
	unsigned char	ar_tha[6];	/* target hardware address	*/
	unsigned int	ar_tip;		/* target IP address		*/
} __attribute__((packed));

typedef struct arphdr arp_t;

struct icmphdr
{
	unsigned char 	type;		/* message type */
	unsigned char 	code;		/* type sub-code */
	unsigned short 	checksum;
	unsigned short	id;
	unsigned short	sequence;
} __attribute__((packed));

typedef struct icmphdr icmp_t;

#define ICMP_ECHOREPLY		0	/* Echo Reply			*/
#define ICMP_ECHO		8	/* Echo Request			*/

/* drivers/dm9000.c */
extern void eth_init(void);
extern int eth_send(const char *buf, u32 length);
extern int eth_recv(char *buf);

/* lib/net.c */
extern unsigned int swapl(unsigned int d);
extern unsigned short swaps(unsigned short d);
extern void ip_to_string(unsigned int x, char *s);
extern unsigned int string_to_ip(const char *s);
extern void string_to_eth(const char *s, unsigned char *);
extern unsigned short cal_sum(unsigned short *buf, int len);

//pack.c
extern int eth_pack(u32, u16);
extern int ip_pack(u32, u8);
extern int udp_pack(u32);

//unpack.c
extern void eth_unpack(u32 length);
extern void ip_unpack(u32 length);
extern void udp_unpack(u32 length);

//ping.c
extern void icmp_unpack(u32 length);
extern void ping(void);

//main.c
extern u8 heth[6];
extern u8 eeth[6];
extern u32 hip;
extern u32 eip;
extern u16 hport;
extern u16 eport;
extern u8 send_buf[2048];
extern u8 recv_buf[2048];

#endif

