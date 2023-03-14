#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

typedef struct node {
    char *name;
    pid_t pid;
    int status; // 1 running, 0 paused
    struct node *next;
} node;

/*
 * pre-error checked malloc, given a number of bytes
 * returns pointer to allocatd memory
 */
void *checked_malloc(size_t);

/*
 * given a "name" and pid creates a node
 * by default sets status to 1 (running)
 * returns pointer to new node
 */
node* new_node(char *, pid_t);

/*
 * adds second node that is passed in to the 
 * end of the first node list, returns pointer 
 * to the head of the list
 */
node* add_back(node *, node *);

/*
 * adds second node to the front of the 
 * list of the first node, returns pointer 
 * to head of the list
 */
node* add_front(node *, node *);

/*
 * removes node with given pid from list
 * if not in list no changes made, returns 
 * pointer to the head of the list
 */
node *remove_pid(node *, pid_t);

/*
 * find node with given pid in list and 
 * change state to int that is passed
 * returns 1 if sucessful and 0 if 
 * pid wasn't found
 */
int change_state(node *, pid_t, int);

/*
 * takes the given list and removes any processes
 * that are no longer running and prints whether 
 * they exited or were killed. general idea is
 * from geeks for geeks:
 * https://www.geeksforgeeks.org/delete-nodes-list-greater-x/ 
 * returns pointer to head of the list
 */
node *update_list(node *);

/*
 * prints all currently active processes and if they are,
 * indicates them as such. prints the number of these processes
 */
void print_list(node *);

/*
 * frees the memory of the node passed in
 */
void free_node(node *);

/*
 * given a list of jobs will kill any running processes and 
 * and free every node in the list, only called when PMan is
 * exited using the exit command
 */
void free_list(node *head);

#endif