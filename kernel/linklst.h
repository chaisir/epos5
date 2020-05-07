#ifndef __LINKLST_H__
#define __LINKLST_H__

struct linknode
{
    void* curr;
    struct linknode* next;
    struct linknode* prev;
};

typedef struct linknode linknode;

linknode* create_head();
linknode* insert_node(void*, linknode*);
void remove_node(linknode*);
void foreach(linknode*, void (void*, void*), void*);
linknode* where(linknode*, int (void*, void*), void*);

#endif