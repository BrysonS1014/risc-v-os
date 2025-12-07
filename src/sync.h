#ifndef SYNC_H
#define SYNC_H

#include "common.h"
#include "scheduler.h"

/* Very small counting semaphore implementation. */

typedef struct semaphore {
    int value;
} semaphore_t;

void sem_init(semaphore_t *s, int initial);
void sem_wait(semaphore_t *s);
void sem_post(semaphore_t *s);

#endif /* SYNC_H */
