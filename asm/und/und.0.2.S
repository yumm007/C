.arch armv5te
.global _start
_start:
	mov r0, #0xBB
	blx r0
	mov pc, lr
