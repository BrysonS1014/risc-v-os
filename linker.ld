OUTPUT_ARCH(riscv)
ENTRY(_start)

/* Simple flat memory layout for QEMU 'virt' machine.
 * DRAM base is 0x80000000 and we assume 128 MiB of RAM.
 */
MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS
{
    /* start at RAM origin so VMAs match the target memory */
    . = ORIGIN(RAM);

    .text : {
        . = ALIGN(4);
        *(.text.boot*)
        *(.text*)
        *(.rodata*)
    } > RAM

    .data : {
        . = ALIGN(8);
        *(.data*)
    } > RAM

    .bss : {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM

    /* Simple single kernel stack (16 KiB) */
    .stack (NOLOAD) : {
        . = ALIGN(16);
        _stack_bottom = .;
        . = . + 0x4000;
        _stack_top = .;
    } > RAM

    /DISCARD/ : {
        *(.comment*)
        *(.eh_frame*)
    }
}
