TinyRV-OS
=========

Authors
--------
Matt Carey
Keith Crase
Ethan Keisler
Bryson Sinclair
ChatGPT/Github Copilot

TinyRV-OS is a **tiny co-operative command-line OS** for a single-core
RISC‑V machine, designed to run on QEMU's `virt` board.

IMPORTANT: THIS IS FOR CSCE-311 AND THE ONLY PURPOSE IS FOR EDUCATION

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

Github Copilot Log:
Start of Github Copilot Log:
– – – – – – – – – – – – – – — – – – – – – — 

Optimizing tool selection...

Adjusting Makefile settings…

Resolving linking errors
I need to address the "relocation truncated to fit: R_RISCV_HI20" linking errors, which happen when large PC-relative jumps exceed the 20-bit immediate range during Auipc/addi sequences. The solution often involves using the -fPIC option, but for baremetal RISC-V, I should consider using -mcmodel=medany instead of the default medlow. RISC-V GCC defaults to medlow for RV64, and switching to medany could help support 64-bit absolute addresses, especially for large kernels. It might also help to add -fno-pic.

Adjusting Makefile settings
I’m planning to inspect the Makefile to adjust LDFLAGS and CFLAGS, adding -mcmodel=medany and potentially -fno-pic. First, I need to remember to use the preamble before tool calls. So, I'll start with managing my to-do list and write down the steps: 1. Inspect the Makefile, 2. Add -mcmodel=medany to both CFLAGS and LDFLAGS, and 3. Re-run make to verify the changes. After that, I’ll read the Makefile and make those small adjustments.
I'll create a short TODO plan, then open Makefile to add -mcmodel=medany to CFLAGS/LDFLAGS and re-run the build.
Created 3 todos

Read Makefile

Preparing to update Makefile

I need to modify CFLAGS and LDFLAGS by adding -mcmodel=medany. I also have to update the Makefile and the todo list statuses — marking step 1 as completed and step 2 as in-progress. I'll use manage_todo_list for this. Then, I’ll apply_patch to update the Makefile with those changes, possibly considering -fno-pic though it might not be necessary. After that, I'll run make using run_in_terminal. Remember to include preamble messages before making each tool call!

Now I'll update Makefile to add -mcmodel=medany to compiler and linker flags. This should allow larger absolute addresses and avoid HI20 relocation truncation.

Starting: Add -mcmodel=medany flags (2/3)

I need to ensure I'm properly using the explanation field and re-run the apply_patch with it. Additionally, I should provide a preamble before making the tool call. I recall giving preamble messages earlier, like before managing the todo list. So, it's important to send a fresh one before apply_patch. The guidelines state I should do this for every tool call. I'll just stick to that and call apply_patch with explanation afterward.

Clarifying tool usage

Makefile+4-2

Updating project tasks
I need to update my to-do list by marking step 2 as completed and step 3 as in progress. I’ll use the manage_todo_list tool for that. After updating, I should run the "make" command to test if the link error has been resolved. It’s important to include a preamble before I run the command like, "Now building the project to verify link error resolved." I’ll make sure functions.run_in_terminal is set with IsBackground as false.

Starting: Build and verify (3/3)

make
Troubleshooting linking issues
I'm facing a persistent linking error, so I'm considering different approaches. I'll look up suggestions like using linker relaxation or the -Wl,-no-relax option, and I see that the error might relate to using -mcmodel=medany with specific linker scripts. Adjusting the script to place .rodata closer to .text could help, but I might also need to explore using -fPIC or -mexplicit-relocs. Now, I'm planning to open linker.ld to check the memory layout.

Now I'll open linker.ld to inspect section placement and check for far-apart rodata/text.

Read linker.ld

Now I'll search the repo for references to R_RISCV_HI20 or discussion about mcmodel.

Searched for regex R_RISCV_HI20|mcmodel=medany|relocation truncated to fit, 20 results
Exploring linker options
Now I'll add -fno-pic to CFLAGS and try building again.

Makefile+1-0

make clean && make -j$(nproc)
Analyzing tool run errors
It seems like the run output truncated before the errors, which is frustrating! I need to see the entire last output since it might give clues. The run returned, but without showing the final lines because of truncation. I think I'll try running the command again, maybe limiting concurrency to just one this time. I’ll run make clean && make, leveraging run_in_terminal. Providing a shorter explanation should help keep things clearer.

make clean && make V=1
Read kernel.c

Implementing changes in code
I'm considering setting the _start label and maybe PHDRS. Adding . = ORIGIN(RAM); is a good idea to ensure the addresses are in the RAM region. I'll patch the code by putting that line at the top of the SECTIONS block or inside the .text section before ALIGN. Next, I should apply the patch and rebuild. I wonder if I also need to update the TODO list at this stage. Time to patch and build!

I'll set the link location to RAM origin in linker.ld to ensure sections are placed at 0x80000000, which can fix HI20 relocation ranges. Then I'll rebuild.

linker.ld+3-0

make clean && make -j$(nproc)

make clean && make -j1 2> build.err || true; echo '=== build.err ==='; sed -n '1,200p' build.err

– – – – – – – – – – – – 
End of Github Copilot log


