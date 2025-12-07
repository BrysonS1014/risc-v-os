TinyRV-OS
=========

Authors
--------
Matt Carey
Keith Crase
Ethan Keisler
Bryson Sinclair
ChatGPT

TinyRV-OS is a **tiny co-operative command-line OS** for a single-core
RISC‑V machine, designed to run on QEMU's `virt` board.

Features
--------

* Bare‑metal RV64 kernel (no libc, no OS underneath)
* Simple UART console driver (ns16550a at `0x10000000`)
* Command‑line shell:
  * `help`, `ls`, `cat`, `ps`, `progs`, `run NAME`
* Co‑operative multitasking:
  * Multiple tasks with separate stacks
  * Round‑robin scheduler
* Synchronization:
  * Simple counting semaphore with blocking/wakeup
  * Demo producer/consumer program (`semdemo`)
* Minimal in‑memory filesystem:
  * Flat list of files (`readme.txt`, `motd.txt`)
* "Programs" as separate tasks:
  * `ping`, `pong`, `semdemo` run concurrently
* Clean separation of kernel subsystems:
  * `kernel.c`, `scheduler.*`, `sync.*`, `fs.*`, `programs.*`, `console.*`, `uart.*`

This is **not** a full Unix; it's a small teaching OS that demonstrates
many core concepts in a compact code base.

Directory layout
----------------

* `linker.ld`      – bare‑metal linker script for QEMU `virt`
* `src/boot.S`     – reset entry, stack setup, BSS clear, calls `kmain`
* `src/context.S`  – context switch primitive
* `src/kernel.c`   – kernel entry, initialization, start scheduler
* `src/uart.*`     – ns16550a UART driver
* `src/console.*`  – text console on top of UART
* `src/kstring.*`  – tiny string/memory helpers
* `src/scheduler.*`– task table, stacks, context switching
* `src/sync.*`     – counting semaphore
* `src/fs.*`       – static in‑memory filesystem
* `src/programs.*` – "user" programs (ping, pong, semdemo)
* `src/shell.*`    – interactive shell
* `Makefile`       – build and run via QEMU

Building
--------

You need:

* A RISC‑V cross compiler (bare‑metal is easiest), for example
  `riscv64-unknown-elf-gcc`.
* QEMU with RISC‑V system emulation (`qemu-system-riscv64`).

On many Linux distributions you can install something like:

* Debian/Ubuntu (package names may vary):

      sudo apt install gcc-riscv64-unknown-elf qemu-system-misc

If your toolchain prefix is different (for example `riscv64-linux-gnu-`),
override `CROSS` when building:

    make CROSS=riscv64-linux-gnu-

To build TinyRV-OS:

    cd tiny-rv-os
    make

This will produce:

* `tinyrv-os.elf` – the kernel image suitable for QEMU `-kernel`.

Running under QEMU
------------------

Run with:

    qemu-system-riscv64 \
        -M virt \
        -m 128M \
        -nographic \
        -bios none \
        -kernel tinyrv-os.elf

Notes:

* `-M virt` selects the generic RISC‑V "virt" machine that provides
  DRAM at `0x80000000`, a CLINT timer and an ns16550a UART at
  `0x10000000`.  
* `-nographic` routes the UART to your terminal.
* `-bios none` tells QEMU to jump straight into our bare‑metal kernel.

ChatGPT Logs:
* NOTE: MUST BE A USC CHATGPT EDU ACCOUNT TO ACCESS
* https://chatgpt.com/g/g-p-6930a5ec61708191bd8e3dd3253baa4c-program-2-cs311/project