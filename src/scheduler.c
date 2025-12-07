#include "scheduler.h"
#include "console.h"

task_t g_tasks[MAX_TASKS];

static context_t boot_ctx;
static task_t *g_current = NULL;

static void task_trampoline(void);

/* Find next READY task after 'from', or return NULL if none. */
static task_t *pick_next(task_t *from)
{
    int start = from ? from->id : 0;
    int i = start;

    for (int n = 0; n < MAX_TASKS; n++) {
        i = (i + 1) % MAX_TASKS;
        if (g_tasks[i].state == TASK_READY) {
            return &g_tasks[i];
        }
    }
    return NULL;
}

task_t *current_task(void)
{
    return g_current;
}

void scheduler_init(void)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        g_tasks[i].id         = i;
        g_tasks[i].state      = TASK_UNUSED;
        g_tasks[i].stack      = NULL;
        g_tasks[i].stack_size = 0;
        g_tasks[i].entry      = NULL;
        g_tasks[i].arg        = NULL;
        g_tasks[i].name       = "(unused)";
        g_tasks[i].waiting_on = NULL;
        g_tasks[i].ctx.ra     = 0;
        g_tasks[i].ctx.sp     = 0;
    }
    g_current = NULL;
}

/* Static stacks for tasks (no dynamic allocation). */
static uint8_t g_task_stacks[MAX_TASKS][TASK_STACK_SIZE];

int task_create(const char *name, void (*entry)(void *), void *arg)
{
    if (!entry) return -1;

    int slot = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (g_tasks[i].state == TASK_UNUSED || g_tasks[i].state == TASK_ZOMBIE) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        console_write("task_create: no free slots\n");
        return -1;
    }

    task_t *t = &g_tasks[slot];

    t->state      = TASK_READY;
    t->stack      = g_task_stacks[slot];
    t->stack_size = TASK_STACK_SIZE;
    t->entry      = entry;
    t->arg        = arg;
    t->name       = name ? name : "task";
    t->waiting_on = NULL;

    /* Initialize context so that when we switch to it, we "return" into
     * task_trampoline() with its own stack.
     */
    uintptr_t sp = (uintptr_t)(t->stack + t->stack_size);
    sp &= ~((uintptr_t)15); /* 16-byte alignment */

    t->ctx.sp  = sp;
    t->ctx.ra  = (uintptr_t)task_trampoline;
    t->ctx.s0  = 0;
    t->ctx.s1  = 0;
    t->ctx.s2  = 0;
    t->ctx.s3  = 0;
    t->ctx.s4  = 0;
    t->ctx.s5  = 0;
    t->ctx.s6  = 0;
    t->ctx.s7  = 0;
    t->ctx.s8  = 0;
    t->ctx.s9  = 0;
    t->ctx.s10 = 0;
    t->ctx.s11 = 0;

    return slot;
}

static int schedule_next(void)
{
    task_t *next = pick_next(g_current);
    if (!next) {
        return 0; /* nothing runnable */
    }

    task_t *prev = g_current;

    next->state = TASK_RUNNING;
    if (prev && prev->state == TASK_RUNNING) {
        prev->state = TASK_READY;
    }

    g_current = next;

    if (prev) {
        context_switch(&prev->ctx, &next->ctx);
    } else {
        /* First time into scheduler: use boot_ctx as previous. */
        context_switch(&boot_ctx, &next->ctx);
    }
    return 1;
}

void scheduler_start(void)
{
    if (g_current != NULL) {
        console_write("scheduler_start: already running\n");
        return;
    }
    if (!pick_next(NULL)) {
        console_write("scheduler_start: no runnable tasks\n");
        return;
    }
    schedule_next();

    /* We should never get here. */
    console_write("scheduler_start: returned unexpectedly\n");
    for (;;)
        ;
}

void task_yield(void)
{
    (void)schedule_next();
}

void task_exit(void)
{
    task_t *t = g_current;
    if (!t) {
        console_write("task_exit: no current task\n");
        for (;;)
            ;
    }

    t->state      = TASK_ZOMBIE;
    t->waiting_on = NULL;

    for (;;) {
        if (!schedule_next()) {
            /* No other runnable tasks: just spin. */
        }
    }
}

void scheduler_dump_tasks(void)
{
    console_write("ID  STATE     NAME\n");
    for (int i = 0; i < MAX_TASKS; i++) {
        task_t *t = &g_tasks[i];
        const char *state = "???";
        switch (t->state) {
        case TASK_UNUSED: state = "UNUSED  "; break;
        case TASK_READY:  state = "READY   "; break;
        case TASK_RUNNING:state = "RUNNING "; break;
        case TASK_BLOCKED:state = "BLOCKED "; break;
        case TASK_ZOMBIE: state = "ZOMBIE  "; break;
        }

        console_write("  ");
        char buf[4];
        buf[0] = '0' + (char)i;
        buf[1] = '\0';
        console_write(buf);
        console_write("  ");
        console_write(state);
        console_write(" ");
        console_write(t->name);
        if (t == g_current) {
            console_write(" (current)");
        }
        console_putc('\n');
    }
}

/* When a new task is first scheduled, we "return" into this. */
static void task_trampoline(void)
{
    task_t *t = g_current;
    if (!t || !t->entry) {
        console_write("task_trampoline: invalid current task\n");
        for (;;)
            ;
    }

    t->entry(t->arg);

    /* If the task function returns, consider it finished. */
    task_exit();
}
