#include <stddef.h>
#include "kernel.h"
#include "linklst.h"

typedef int bool;
#define true 1
#define false 0

typedef struct linknode linknode;

linknode* create_node(void* item)
{
    linknode* node = kmalloc(sizeof(void*) * 3);
    node->curr = item;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void* delete_node(linknode* node)
{
    void* item = node->curr;
    kfree(node);
    return item;
}

inline bool node_is_head(linknode* node)
{
    return node->prev == NULL;
}

linknode* create_head()
{
    return create_node(NULL);
}

linknode* insert_node(void* item, linknode* position)
{
    if(position == NULL) { return NULL; }
    linknode* n = create_node(item);
    n->next = position->next;
    n->prev = position;
    if(position->next != NULL)
    {
        position->next->prev = n;
    }
    position->next = n;
    return n;
}

void remove_node(linknode* node)
{
    if(node == NULL){ return; }
    if(node_is_head(node))
    {
        while(node != NULL)
        {
            linknode *next = node->next;
            delete_node(node);
            node = next;
        }
    }
    else
    {
        node->prev->next = node->next;
        if(node->next != NULL)
        {
            node->next->prev = node->prev;
        }
        delete_node(node);
    }
}

linknode* goto_head_node(linknode* node)
{
    if(node == NULL) { return NULL; }
    while(!node_is_head(node))
    {
        node = node->prev;
    }
    return node;
}

void foreach(linknode* head, void (func)(void*, void*), void* initState)
{
    if(head == NULL) { return; }
    linknode* node = goto_head_node(head)->next;
    while(node != NULL)
    {
        func(node->curr, initState);
        node = node->next;
    }
}

linknode* where(linknode* head, int (predicate)(void*, void*), void* initState)
{
    if(head == NULL) { return NULL; }
    linknode* node = goto_head_node(head)->next;
    while(node != NULL)
    {
        bool result = predicate(node->curr, initState) != 0;
        if(result == true)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
}