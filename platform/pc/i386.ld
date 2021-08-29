OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_kernel_start)

SECTIONS
{
    . = 0x100000;
    _kernel_start = .;
    .mboot :    { *(.text.init*) KEEP(*(.text.init)) }
    .text :     { *(.text*)      KEEP(*(.text))      }
    .rodata :   { *(.rodata*)    KEEP(*(.rodata))    }
    .data :     { *(.data*)      KEEP(*(.data))      }
    _bss_start = .;
    .bss :      { *(.bss*)       KEEP(*(.bss))       }
    _bss_end = .;
    .cdi :      { *(.cdi*)       KEEP(*(.cdi))       }
    /DISCARD/ : { *(.eh_frame*);                     }
    _kernel_end = .;
}