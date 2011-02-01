.global _start

_start:
	mov r0, #0
	MRC p15,0,r0,c0,c0,1
	mov pc, lr

