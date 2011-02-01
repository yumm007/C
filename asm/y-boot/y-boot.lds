OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm");
OUTPUT_ARCH(arm);
ENTRY(_start);

SECTIONS
{
	. = 0x33f80000;
	
	. = ALIGN(4);
	.text : {
		start/start.o (.text);
		start/nand.o (.text);
		*(.text);
	}

	. = ALIGN(4);
	.rodata : {
		*(.rodata);
	}

	. = ALIGN(4);
	.data : {
		*(.data);
	}

	. = ALIGN(4);
	fun_start = .;
	.fun_section : {
		*(.fun_section);
	}
	fun_end = .;

   . = ALIGN(4);
	bss_start = .;  
   .bss : {
		 *(.bss) 
		. = ALIGN(4);
	}
   bss_end = .;

}


