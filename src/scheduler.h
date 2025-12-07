#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "common.h"

typedef enum {
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_ZOMBIE
} task_state_t;

typedef struct context {
    uint64_t ra;
    uint64_t sp;
    uint64_t s0;
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
} context_t;

struct task;

typedef struct task {
    int          id;
    task_state_t state;
    context_t    ctx;
    uint8_t     *stack;
    size_t       stack_size;
    void       (*entry)(void *arg);
    void        *arg;
    const char  *name;
    void        *waiting_on; /* generic wait object pointer (e.g., semaphore) */
} task_t;

#define MAX_TASKS       8
#define TASK_STACK_SIZE 4096

extern task_t g_tasks[MAX_TASKS];

void scheduler_init(void);
int  task_create(const char *name, void (*entry)(void *), void *arg);
void scheduler_start(void);
void task_yield(void);
void task_exit(void);
task_t *current_task(void);
void scheduler_dump_tasks(void);

/* Low-level context switch implemented in assembly. */
void context_switch(context_t *old_ctx, context_t *new_ctx);

#endif /* SCHEDULER_H */
