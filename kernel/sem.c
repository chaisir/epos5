/**
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <stddef.h>
#include "kernel.h"
#include "linklst.h"

typedef struct linknode linknode;
typedef struct semaphore
{
    int id;
    int value;
    struct wait_queue *queue;
} semaphore;

linknode semhead = { NULL, NULL, NULL };
int candidateId = 0;
int sem_id_selector(void*, void*);

int sys_sem_create(int value)
{
    void* pt = kmalloc(sizeof(int) * 2 + sizeof(void*));
    if(pt == NULL) { return -1; }
    semaphore* sem = (semaphore*)pt;
    sem->id = candidateId;
    sem->value = value;
    sem->queue = NULL;
    candidateId++;
    insert_node(sem, &semhead);
    return sem->id;
}

int sys_sem_destroy(int semid)
{
    linknode* node = where(&semhead, &sem_id_selector, &semid);
    if(node == NULL) { return -1; }
    kfree(node->curr);
    remove_node(node);
    return 0;
}

int sys_sem_wait(int semid)
{
    linknode* node = where(&semhead, &sem_id_selector, &semid);
    if(node == NULL) { return -1; }
    semaphore* sem = (semaphore*)(node->curr);
    uint32_t flags;
    save_flags_cli(flags);
    sem->value--;
    if(sem->value < 0)
    {
        sleep_on(&(sem->queue));
    }
    restore_flags(flags);
    return 0;
}

int sys_sem_signal(int semid)
{
    linknode* node = where(&semhead, &sem_id_selector, &semid);
    if(node == NULL) { return -1; }
    semaphore* sem = (semaphore*)(node->curr);
    uint32_t flags;
    save_flags_cli(flags);
    if(sem->value < 0)
    {
        wake_up(&(sem->queue), 1);
    }
    sem->value++;
    restore_flags(flags);
    return 0;
}

int sem_id_selector(void* pt, void* state)
{
    if(pt == NULL) { return 0; }
    int targetId = *((int*)state);
    semaphore* sem = (semaphore*)pt;
    if(sem->id == targetId) { return 1; }
    else { return 0; }
}