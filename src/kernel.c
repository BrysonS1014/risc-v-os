#include "common.h"
#include "console.h"
#include "scheduler.h"
#include "fs.h"
#include "shell.h"

void panic(const char *msg)
{
    console_write("\nPANIC: ");
    if (msg) console_write(msg);
    console_putc('\n');
    for (;;)
        ;
}

void kmain(void)
{
    console_init();

    console_write("\n=== TinyRV-OS ===\n");
    console_write("A tiny co-operative command-line OS for RISC-V (QEMU virt)\n");
    console_write("---------------------------------------------------------\n");

    scheduler_init();
    fs_init();

    if (task_create("shell", shell_main, NULL) < 0) {
        panic("failed to create shell task");
    }

    scheduler_start();

    panic("scheduler_start returned unexpectedly");
}
