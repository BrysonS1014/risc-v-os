#include "shell.h"
#include "console.h"
#include "fs.h"
#include "scheduler.h"
#include "programs.h"
#include "kstring.h"

static char *skip_spaces(char *s)
{
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

void shell_main(void *arg)
{
    (void)arg;

    const fs_node_t *motd = fs_find("motd.txt");
    if (motd) {
        fs_cat("motd.txt");
    }

    console_write("Type 'help' for a list of commands.\n");

    char line[128];

    for (;;) {
        console_write("\n$ ");
        console_readline(line, sizeof(line));

        char *p = skip_spaces(line);
        if (*p == '\0') {
            continue;
        }

        /* Extract first word */
        char *cmd = p;
        while (*p && *p != ' ' && *p != '\t') {
            p++;
        }
        if (*p) {
            *p++ = '\0';
        }
        p = skip_spaces(p);

        if (k_strcmp(cmd, "help") == 0) {
            console_write("Commands:\n");
            console_write("  help          - show this help\n");
            console_write("  ls            - list files\n");
            console_write("  cat NAME      - show file\n");
            console_write("  ps            - list tasks\n");
            console_write("  progs         - list demo programs\n");
            console_write("  run NAME      - start a demo program\n");
            console_write("  exit          - (shell ignores this; keep running)\n");
        } else if (k_strcmp(cmd, "ls") == 0) {
            fs_list();
        } else if (k_strcmp(cmd, "cat") == 0) {
            if (*p == '\0') {
                console_write("cat: missing filename\n");
            } else {
                fs_cat(p);
            }
        } else if (k_strcmp(cmd, "ps") == 0) {
            scheduler_dump_tasks();
        } else if (k_strcmp(cmd, "progs") == 0) {
            programs_list();
        } else if (k_strcmp(cmd, "run") == 0) {
            if (*p == '\0') {
                console_write("run: missing program name\n");
            } else {
                programs_start(p);
            }
        } else if (k_strcmp(cmd, "exit") == 0) {
            console_write("Shell won't exit in this demo. Use Ctrl+A, X in QEMU to quit.\n");
        } else {
            console_write("Unknown command: ");
            console_write(cmd);
            console_putc('\n');
        }

        /* Give other tasks a chance to run. */
        task_yield();
    }
}
