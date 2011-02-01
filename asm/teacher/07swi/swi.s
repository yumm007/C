
.global _start
_start:
	stmfd	sp!, {lr}	

	swi	#3


	ldmfd	sp!, {pc}

