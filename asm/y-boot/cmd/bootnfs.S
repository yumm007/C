@内核启动时调用的首函数地址0x30008040(连接是指定)需要三个参数,第一个为0,第2个CPU类型MACH_TYPE_S3C2440.第三个为一链表地址
@当为lunux内核时,此链表至少有三个结果体.每个结构体第一个值为此结构体的大小/4, 第二个为此结构体的类型,后面的内容依据此类型读取
@第一个结构体为头,第二个为传给linux内核的参数,第三个结构体全为0,表示到了链表尾.

	.section .fun_section,"aw"
bootm_fun_a:
		.word cmd_name	
		.word bootnfs

	.data
cmd_name:
		.string	"bootnfs"

	.align 2
struct_start:
	.word	 5
	.word	 0x54410001
	.word	 0
	.word	 0
	.word	 0

	.word  	29
	.word	 	0x54410009
	.string  "noinitrd console=ttySAC0,115200 mem=64M root=/dev/nfs rw nfsroot=192.168.1.71:/nfsroot  ip=192.168.1.70"

	.align 2
	.word  0
	.word 0x00000000
struct_end:

	.align 2
	.text
.global bootnfs
bootnfs:
	ldr r0, =struct_start
	ldr r1, =0x30000100
l:
	ldrb r2, [r0], #1
	strb r2, [r1], #1
	ldr r3, =struct_end
	cmp r0, r3
	bne l
	
	mov r0, #0
	ldr r1, =362
	ldr r2, =0x30000100
	ldr pc, =0x30008040

