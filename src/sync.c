#include "sync.h"

void sem_init(semaphore_t *s, int initial)
{
    if (!s) return;
    s->value = initial;
}

void sem_wait(semaphore_t *s)
{
    if (!s) return;

    for (;;) {
        if (s->value > 0) {
            s->value--;
            return;
        }

        task_t *t = current_task();
        if (!t) {
            return;
        }

        t->state      = TASK_BLOCKED;
        t->waiting_on = s;

        task_yield();
        /* When we resume, try again. */
    }
}

void sem_post(semaphore_t *s)
{
    if (!s) return;

    s->value++;

    /* Wake a single task waiting on this semaphore, if any. */
    for (int i = 0; i < MAX_TASKS; i++) {
        task_t *t = &g_tasks[i];
        if (t->state == TASK_BLOCKED && t->waiting_on == s) {
            t->waiting_on = NULL;
            t->state      = TASK_READY;
            break;
        }
    }
}
