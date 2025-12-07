#include "programs.h"
#include "console.h"
#include "kstring.h"
#include "scheduler.h"
#include "sync.h"

/* A couple of tiny demo "user programs" that show scheduling and
 * synchronization.  They are all simple C functions that run as
 * separate tasks in the same address space.
 */

static semaphore_t g_sem;
static int g_sem_initialized = 0;

static void prog_ping(void *arg)
{
    (void)arg;
    for (;;) {
        console_write("[ping] hello from ping task\n");
        task_yield();
    }
}

static void prog_pong(void *arg)
{
    (void)arg;
    for (;;) {
        console_write("[pong] hello from pong task\n");
        task_yield();
    }
}

/* Semaphore demo: producer and consumer coordinate via g_sem. */

static void sem_producer(void *arg)
{
    (void)arg;
    for (;;) {
        console_write("[sem-producer] producing item\n");
        sem_post(&g_sem);
        task_yield();
    }
}

static void sem_consumer(void *arg)
{
    (void)arg;
    for (;;) {
        console_write("[sem-consumer] waiting for item...\n");
        sem_wait(&g_sem);
        console_write("[sem-consumer] got item\n");
        task_yield();
    }
}

/* "semdemo" program: spawns producer and consumer, then exits. */

static void prog_semdemo(void *arg)
{
    (void)arg;

    if (!g_sem_initialized) {
        sem_init(&g_sem, 0);
        g_sem_initialized = 1;
    }

    if (task_create("sem-prod", sem_producer, NULL) < 0) {
        console_write("semdemo: failed to create producer\n");
    }
    if (task_create("sem-cons", sem_consumer, NULL) < 0) {
        console_write("semdemo: failed to create consumer\n");
    }

    console_write("semdemo: started producer and consumer\n");
    task_exit();
}

typedef struct {
    const char *name;
    void (*entry)(void *arg);
} program_desc_t;

static const program_desc_t g_programs[] = {
    { "ping",    prog_ping    },
    { "pong",    prog_pong    },
    { "semdemo", prog_semdemo },
};

void programs_list(void)
{
    console_write("Programs:\n");
    for (size_t i = 0; i < ARRAY_SIZE(g_programs); i++) {
        console_write("  ");
        console_write(g_programs[i].name);
        console_putc('\n');
    }
}

int programs_start(const char *name)
{
    if (!name || !name[0]) {
        console_write("run: missing program name\n");
        return -1;
    }

    for (size_t i = 0; i < ARRAY_SIZE(g_programs); i++) {
        if (k_strcmp(g_programs[i].name, name) == 0) {
            if (task_create(g_programs[i].name, g_programs[i].entry, NULL) < 0) {
                console_write("run: failed to create task\n");
                return -1;
            }
            return 0;
        }
    }

    console_write("run: no such program: ");
    console_write(name);
    console_putc('\n');
    return -1;
}
