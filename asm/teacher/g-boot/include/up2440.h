#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONFIG_COM1

#define CONFIG_SYS_CBSIZE 512

#define CONFIG_SYS_PBSIZE 1024

#define CONFIG_SYS_PROMPT "[g-boot]#"

#define CONFIG_SYS_MAXARGS 10

#define CONFIG_IPADDR "192.168.1.8"

#define CONFIG_SERVERIP "192.168.1.3"

#define CONFIG_ETHADDR "11:22:33:44:55:66"

#define CONFIG_BOOTARGS "noinitrd console=ttySAC0,115200 mem=64M root=/dev/nfs rw nfsroot=192.168.1.3:/nfsroot ip=192.168.1.8"

#endif
