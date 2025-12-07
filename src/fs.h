#ifndef FS_H
#define FS_H

#include "common.h"

typedef enum {
    FS_NODE_FILE = 1,
} fs_node_type_t;

typedef struct fs_node {
    const char      *name;
    fs_node_type_t   type;
    const char      *data;
    size_t           size;
} fs_node_t;

extern const fs_node_t fs_root[];
extern const size_t    fs_root_count;

void fs_init(void);
void fs_list(void);
const fs_node_t *fs_find(const char *name);
void fs_cat(const char *name);

#endif /* FS_H */
