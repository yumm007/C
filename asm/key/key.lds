OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm");
OUTPUT_ARCH(arm);
ENTRY(main);

SECTIONS
{
	. = 0x30000000;
	
	. = ALIGN(4);
	.text : {
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
	.bss : {
		*(.bss);
	}
}


