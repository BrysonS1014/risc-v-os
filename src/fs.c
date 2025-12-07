#include "fs.h"
#include "console.h"
#include "kstring.h"

/* Very small in-memory "filesystem".
 * Everything lives in a single flat root directory.
 */

static const char readme_txt[] =
    "TinyRV-OS demo filesystem.\n"
    "Commands:\n"
    "  help       - show commands\n"
    "  ls         - list files\n"
    "  cat NAME   - show file contents\n"
    "  ps         - show tasks\n"
    "  progs      - list runnable demo programs\n"
    "  run NAME   - start demo program\n"
    "\n";

static const char motd_txt[] =
    "Welcome to TinyRV-OS running on QEMU RISC-V 'virt'.\n";

const fs_node_t fs_root[] = {
    { "readme.txt", FS_NODE_FILE, readme_txt, sizeof(readme_txt) - 1 },
    { "motd.txt",   FS_NODE_FILE, motd_txt,   sizeof(motd_txt) - 1 },
};

const size_t fs_root_count = ARRAY_SIZE(fs_root);

void fs_init(void)
{
    /* Nothing to do for static in-memory filesystem. */
}

void fs_list(void)
{
    console_write("FILES:\n");
    for (size_t i = 0; i < fs_root_count; i++) {
        console_write("  ");
        console_write(fs_root[i].name);
        console_putc('\n');
    }
}

const fs_node_t *fs_find(const char *name)
{
    if (!name) return NULL;
    for (size_t i = 0; i < fs_root_count; i++) {
        if (k_strcmp(fs_root[i].name, name) == 0) {
            return &fs_root[i];
        }
    }
    return NULL;
}

void fs_cat(const char *name)
{
    const fs_node_t *n = fs_find(name);
    if (!n) {
        console_write("No such file: ");
        console_write(name);
        console_putc('\n');
        return;
    }
    if (n->type != FS_NODE_FILE) {
        console_write("Not a regular file: ");
        console_write(name);
        console_putc('\n');
        return;
    }

    for (size_t i = 0; i < n->size; i++) {
        console_putc(n->data[i]);
    }
    if (n->size == 0 || n->data[n->size - 1] != '\n') {
        console_putc('\n');
    }
}
