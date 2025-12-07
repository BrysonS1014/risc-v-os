/* Minimal RISC-V RV64 boot code for QEMU 'virt' machine.
 *
 * QEMU loads the ELF at 0x80000000 and jumps to _start.
 * We set up a stack and clear .bss, then call into C kmain().
 */

    .section .text.boot, "ax"
    .globl _start
    .type _start, @function

    .extern kmain
    .extern __bss_start
    .extern __bss_end
    .extern _stack_top

_start:
    /* Set stack pointer */
    la      sp, _stack_top

    /* Zero out .bss */
    la      t0, __bss_start
    la      t1, __bss_end

1:
    bgeu    t0, t1, 2f
    sd      zero, 0(t0)
    addi    t0, t0, 8
    j       1b

2:
    /* Jump into C kernel */
    call    kmain

    /* If kmain ever returns, just spin. */
3:
    j       3b

    .size _start, .-_start
