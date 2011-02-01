OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x33f80000;

	. = ALIGN(4);
	.text      :
	{
	  kernel/start.o	(.text)
	  kernel/clock_init.o	(.text)
	  kernel/mem_init.o	(.text)
	  kernel/nand_read.o	(.text)
	  *(.text)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	__g_boot_cmd_start = .;
	.g_boot_cmd : { *(.g_boot_cmd) }
	__g_boot_cmd_end = .;

	. = ALIGN(4);
	__bss_start = .;	
	.bss : { *(.bss) . = ALIGN(4);}
	__bss_end = .;
}

