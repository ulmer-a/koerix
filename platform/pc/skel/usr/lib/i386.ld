OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    . = 0x01000000;
    .text	: { *(.text) 	}
    .rodata	: { *(.rodata*) }
    . = ALIGN(4096);
    .data	: { *(.data*) 	}
    .bss	: { *(.bss*)	}
}
