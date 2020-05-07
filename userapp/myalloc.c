/**
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <syscall.h>
#include <stdio.h>

struct chunk
{
    char signature[4];  /* "OSEX" */
    struct chunk *next; /* ptr. to next chunk */
    int state;          /* 0 - free, 1 - used */
#define FREE 0
#define USED 1

    int size; /* size of this chunk */
};

struct chunk create_chunk(int size)
{
    struct chunk c;
    strncpy(c.signature, "OSEX", 4);
    c.next = NULL;
    c.state = FREE;
    c.size = size;
    return c;
}

int semId = 0;

static struct chunk *chunk_head;

void *g_heap;
void *tlsf_create_with_pool(uint8_t *heap_base, size_t heap_size)
{
    semId = sem_create(1);
    sem_wait(semId);
    chunk_head = (struct chunk *)heap_base;
    strncpy(chunk_head->signature, "OSEX", 4);
    chunk_head->next = NULL;
    chunk_head->state = FREE;
    chunk_head->size = heap_size;
    sem_signal(semId);

    return NULL;
}

struct chunk* get_free(size_t size)
{
    struct chunk* c = chunk_head;
    while(c != NULL)
    {
        if(c->state == FREE && c->size >= size)
        {
            return c;
        }
        c = c->next;
    }
    return NULL;
}
void compact()
{
    struct chunk* c0 = chunk_head;
    while(c0 != NULL)
    {
        struct chunk* c1 = c0->next;
        if(c0->state == FREE)
        {
            int size = c0->size;
            while(c1 != NULL && c1->state == FREE)
            {
                size += (c1->size + sizeof(struct chunk));
                c1 = c1->next;
            }
            c0->size = size;
            c0->next = c1;
        }
        c0 = c1;
    }
}

inline struct chunk* get_chunk(void* ptr)
{
    if(ptr != NULL)
    {
        struct chunk *achunk = (struct chunk *)(((uint8_t *)ptr) - (sizeof(struct chunk)));
        if(strncmp(achunk->signature, "OSEX", 4) == 0)
        {
            return achunk;
        }
    }
    return NULL;
}
inline void* get_ptr(struct chunk* chk)
{
    if(chk != NULL)
    {
        if(strncmp(chk->signature, "OSEX", 4) == 0)
        {
            return (uint8_t *)chk + sizeof(struct chunk);
        }
    }
    return NULL;
}

void *malloc(size_t size)
{
    if(size == 0) 
    {     
        return NULL; 
    }

    sem_wait(semId);
    struct chunk* c = get_free(size);
    if(c != NULL)
    {
        void* ptr = get_ptr(c);
        int remainingSize = c->size - size - sizeof(struct chunk);
        if(remainingSize > 0)
        {
            struct chunk* newHeader = (struct chunk*)((uint8_t *)ptr + size);
            *newHeader = create_chunk(remainingSize);
            newHeader->next = c->next;
            c->next = newHeader;
            c->state = USED;
            c->size = size;
            sem_signal(semId);
            return ptr;
        }
        else
        {
            c->state = USED;
            sem_signal(semId);
            return ptr;
        }
    }
    else
    {
        sem_signal(semId);
        return NULL;
    }
}

void free(void *ptr)
{
    if (ptr == NULL) { return; }

    sem_wait(semId);
    struct chunk* c = get_chunk(ptr);
    if(c != NULL)
    {
        c->state = FREE;
        compact();
    }
    sem_signal(semId);
}

void *calloc(size_t num, size_t size)
{
    int arraySize = num * size;
    void* ptr = malloc(arraySize);
    if(ptr == NULL)
    {
        return NULL;
    }
    else
    {
        int i = 0;
        for(; i < arraySize; i++)
        {
            *((uint8_t *)ptr + i) = 0;
        }
        return ptr;
    }
}

void *realloc(void *oldptr, size_t size)
{
    if(size == 0)
    {
        free(oldptr);
        return NULL;
    }
    else
    {
        void *ptr = malloc(size);
        if (oldptr != NULL)
        {
            struct chunk *oldChunk = get_chunk(oldptr);
            if (oldChunk != NULL)
            {
                int copySize = (oldChunk->size > size) ? size : oldChunk->size;
                int i = 0;
                for(; i < copySize; i++)
                {
                    *((uint8_t *)ptr + i) = *((uint8_t *)oldptr + i);
                }
                free(oldptr);
                return ptr;
            }
        }
        return ptr;
    }
}

/*************D O  N O T  T O U C H  A N Y T H I N G  B E L O W*************/
static void tsk_malloc(void *pv)
{
    int i, c = (int)pv;
    char **a = malloc(c * sizeof(char *));
    for (i = 0; i < c; i++)
    {
        a[i] = malloc(i + 1);
        a[i][i] = 17;
    }
    for (i = 0; i < c; i++)
    {
        free(a[i]);
    }
    free(a);

    task_exit(0);
}

#define MESSAGE(foo) printf("%s, line %d: %s", __FILE__, __LINE__, foo)
void test_allocator()
{
    char *p, *q, *t;

    MESSAGE("[1] Test malloc/free for unusual situations\r\n");

    MESSAGE("  [1.1]  Allocate small block ... ");
    p = malloc(17);
    if (p == NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    p[0] = p[16] = 17;
    printf("PASSED\r\n");

    MESSAGE("  [1.2]  Allocate big block ... ");
    q = malloc(4711);
    if (q == NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    q[4710] = 47;
    printf("PASSED\r\n");

    MESSAGE("  [1.3]  Free big block ... ");
    free(q);
    printf("PASSED\r\n");

    MESSAGE("  [1.4]  Free small block ... ");
    free(p);
    printf("PASSED\r\n");

    MESSAGE("  [1.5]  Allocate huge block ... ");
    q = malloc(32 * 1024 * 1024 - sizeof(struct chunk));
    if (q == NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    q[32 * 1024 * 1024 - sizeof(struct chunk) - 1] = 17;
    free(q);
    printf("PASSED\r\n");

    MESSAGE("  [1.6]  Allocate zero bytes ... ");
    if ((p = malloc(0)) != NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    printf("PASSED\r\n");

    MESSAGE("  [1.7]  Free NULL ... ");
    free(p);
    printf("PASSED\r\n");

    MESSAGE("  [1.8]  Free non-allocated-via-malloc block ... ");
    int arr[5] = {0x55aa4711, 0x5a5a1147, 0xa5a51471, 0xaa551741, 0x5aa54171};
    free(&arr[4]);
    if (arr[0] == 0x55aa4711 &&
        arr[1] == 0x5a5a1147 &&
        arr[2] == 0xa5a51471 &&
        arr[3] == 0xaa551741 &&
        arr[4] == 0x5aa54171)
    {
        printf("PASSED\r\n");
    }
    else
    {
        printf("FAILED\r\n");
        return;
    }

    MESSAGE("  [1.9]  Various allocation pattern ... ");
    int i;
    size_t pagesize = sysconf(_SC_PAGESIZE);
    for (i = 0; i < 7411; i++)
    {
        p = malloc(pagesize);
        p[pagesize - 1] = 17;
        q = malloc(pagesize * 2 + 1);
        q[pagesize * 2] = 17;
        t = malloc(1);
        t[0] = 17;
        free(p);
        free(q);
        free(t);
    }

    char **a = malloc(2741 * sizeof(char *));
    for (i = 0; i < 2741; i++)
    {
        a[i] = malloc(i + 1);
        a[i][i] = 17;
    }
    for (i = 0; i < 2741; i++)
    {
        free(a[i]);
    }
    free(a);

    if (chunk_head->next != NULL || chunk_head->size != 32 * 1024 * 1024)
    {
        printf("FAILED\r\n");
        return;
    }
    printf("PASSED\r\n");

    MESSAGE("  [1.10] Allocate using calloc ... ");
    int *x = calloc(17, 4);
    for (i = 0; i < 17; i++)
        if (x[i] != 0)
        {
            printf("FAILED\r\n");
            return;
        }
        else
            x[i] = i;
    free(x);
    printf("PASSED\r\n");

    MESSAGE("[2] Test realloc() for unusual situations\r\n");

    MESSAGE("  [2.1]  Allocate 17 bytes by realloc(NULL, 17) ... ");
    p = realloc(NULL, 17);
    if (p == NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    p[0] = p[16] = 17;
    printf("PASSED\r\n");
    MESSAGE("  [2.2]  Increase size by realloc(., 4711) ... ");
    p = realloc(p, 4711);
    if (p == NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    if (p[0] != 17 || p[16] != 17)
    {
        printf("FAILED\r\n");
        return;
    }
    p[4710] = 47;
    printf("PASSED\r\n");

    MESSAGE("  [2.3]  Decrease size by realloc(., 17) ... ");
    p = realloc(p, 17);
    if (p == NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    if (p[0] != 17 || p[16] != 17)
    {
        printf("FAILED\r\n");
        return;
    }
    printf("PASSED\r\n");

    MESSAGE("  [2.4]  Free block by realloc(., 0) ... ");
    p = realloc(p, 0);
    if (p != NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    else
        printf("PASSED\r\n");

    MESSAGE("  [2.5]  Free block by realloc(NULL, 0) ... ");
    p = realloc(realloc(NULL, 0), 0);
    if (p != NULL)
    {
        printf("FAILED\r\n");
        return;
    }
    else
        printf("PASSED\r\n");

    MESSAGE("[3] Test malloc/free for thread-safe ... ");

    int t1, t2;
    char *s1 = malloc(1024 * 1024),
         *s2 = malloc(1024 * 1024);
    t1 = task_create(s1 + 1024 * 1024, tsk_malloc, (void *)5000);
    t2 = task_create(s2 + 1024 * 1024, tsk_malloc, (void *)5000);
    task_wait(t1, NULL);
    task_wait(t2, NULL);
    free(s1);
    free(s2);

    if (chunk_head->next != NULL || chunk_head->size != 32 * 1024 * 1024)
    {
        printf("FAILED\r\n");
        return;
    }
    printf("PASSED\r\n");
}
/*************D O  N O T  T O U C H  A N Y T H I N G  A B O V E*************/
