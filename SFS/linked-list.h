#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

/*
 * linked list based off Mike Zastre seng265-list.[ch] files
 */

#include "input-structs.c"

typedef struct node_t {
    entry_t entry;
    struct node_t  *next;
} node_t;

node_t *new_node(entry_t);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void free_list(node_t *);
void    apply(node_t *, void(*fn)(node_t *, void *), void *arg);

#endif
