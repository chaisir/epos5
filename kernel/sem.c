/**
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <stddef.h>
#include "kernel.h"

struct Semaphore *get_semaphore(int semid);
void add_semaphore(struct Semaphore *sem);
void remove_semaphore(struct Semaphore *sem);

int sys_sem_create(int value){
    static int semid=0;
    struct Semaphore *sem=(struct Semaphore *)kmalloc(sizeof(struct Semaphore));
    sem->value=value;
    sem->next=NULL;
    sem->semid=semid++;
    sem->waitqueue=NULL;
    add_semaphore(sem);
    if(sem==NULL)
        return -1;
    else
        return sem->semid;
}

int sys_sem_destroy(int semid){
    struct Semaphore *sem;
    sem=get_semaphore(semid);
    if(sem==NULL)
        return -1;
    remove_semaphore(sem);
    kfree(sem);
    return 0;
}

int sys_sem_wait(int semid){
	struct Semaphore *sem;
	sem=get_semaphore(semid);
	if(sem==NULL)
		return -1;
	uint32_t flags;
    save_flags_cli(flags);
    sem->value--;
    if(sem->value<0)
		sleep_on(&sem->waitqueue);
    restore_flags(flags);
    return 0;
}

int sys_sem_signal(int semid){
	struct Semaphore *sem;
	sem=get_semaphore(semid);
	if(sem==NULL)
		return -1;
	uint32_t flags;
    save_flags_cli(flags);
    sem->value++;
    if(sem->value<=0)
		wake_up(&sem->waitqueue,1);
    restore_flags(flags);
    return 0;
}

struct Semaphore *sem_first=NULL;

void add_semaphore(struct Semaphore *sem){
    if(sem_first==NULL)
        sem_first=sem;
    else{
        struct Semaphore *a,*b;
        a=sem_first;
        while(1){
            b=a;
            a=a->next;
            if(a==NULL){
                b->next=sem;
                break;
            }
        }
    }
}

struct Semaphore *get_semaphore(int semid){
    struct Semaphore *sem;
    sem=sem_first;
    while(sem!=NULL){
        if(sem->semid==semid)
            break;
        sem=sem->next;
    }
    return sem;
}


void remove_semaphore(struct Semaphore *sem){
    if(sem_first!=NULL){
        if(sem==sem_first)
            sem_first=sem_first->next;
        else{
            struct Semaphore *a,*b;
            a=sem_first;
            while(a!=NULL){
                b=a;
                a=a->next;
                if(a==sem){
                    b->next=a->next;
                    break;
                }
            }
        }
    }
}

