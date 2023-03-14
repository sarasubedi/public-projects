#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#include "linked-list.h"

void *checked_malloc(size_t bytes) {
    void* p;
    p = malloc(bytes);
    if(!p) {
        fprintf(stderr, "malloc failed!");
        exit(0);
    }
    return p;
}

node *new_node(char *name, pid_t pid) {
    node *p = (node *)checked_malloc(sizeof(node));

    // copy passed in info into node
    p->name = checked_malloc(sizeof(char)*(strlen(name)+1));
    strncpy(p->name, name, strlen(name)+1);
    p->pid = pid;
    p->status = 1;
    p->next = NULL;

    return p;
}

node *add_back(node *head, node *last) {
    last->next = NULL;
    if(!head) return last; // if list empty
    node* cur = head;
    for( ; cur->next != NULL; cur = cur->next)
    cur->next = last;
    return head;
}

node *add_front(node *head, node *first) {
    if(!head) return first; // if list empty
    first->next = head;
    return first;
}

node *remove_pid(node *head, pid_t pid) {
    if(!head) {
        return NULL; // pid wasn't in list
    } else if (head->pid == pid) { 
        // pid is head
        node *temp = head->next;
        free(head);
        return temp;
    }
    node *cur = head;
    node *last = cur;
    for( ; cur->pid != pid && cur != NULL; last = cur, cur = cur->next)
    if(!cur) return head; // pid wasn't in list
    last->next = cur->next;
    free_node(cur);

    return head;
}

int change_state(node *head, pid_t pid, int state) {
    if(!head) {
        return 0; // pid wasn't in list
    } else if (head->pid == pid) {
        // pid is head
        head->status = state;
        return 1;
    }
    node *cur = head;
    for( ; cur->pid != pid && cur != NULL; cur = cur->next)
    if(!cur) return 0; // pid wasn't in list
    cur->status = state;
    return 1;
}

node *update_list(node *head) {
    if(!head) return NULL;
    node *cur = head;
    node *prev = NULL;
    int status;
    printf("\n");
    // sets head to be next running process if there
    // is any. if head is running this loop is skipped.
    while(waitpid(cur->pid, &status, WNOHANG)) {
        head = head->next;
        if(WIFSIGNALED(status)){
            printf("Process [%d] was killed\n", cur->pid);
        } else if(WIFEXITED(status)) {
            printf("Process [%d] exited\n", cur->pid);
        }
        free_node(cur);
        cur = head;
        if(!head) return NULL;
    }   
    while(cur != NULL) { // checks whole list
        while (cur != NULL && !waitpid(cur->pid, &status, WNOHANG)) {
            // continues through list if cur is running
            prev = cur;
            cur = cur->next;
        }
        if(!cur) return head; // made it to end of list
        prev->next = cur->next;
        if(WIFSIGNALED(status)){
            printf("Process [%d] killed\n", cur->pid);
        } else if(WIFEXITED(status)) {
            printf("Process [%d] exited\n", cur->pid);
        }
        free_node(cur);
        cur = prev->next;
    }
    return head;
}

void print_list(node *head) {
    int counter = 0;
    for(node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d: %s", cur->pid, cur->name);
        counter++;
        if(!cur->status) { // check if status is paused
            printf("\t(PAUSED)");
        }
        printf("\n");
    }
    printf("\n");
    
    printf("Total background jobs: %d\n\n", counter);
}

void free_node(node *rm) {
    free(rm->name);
    free(rm);
}

void free_list(node *head) {
    node *temp;
    for(node *cur = head; cur != NULL; cur = temp) {
        temp = cur->next;
        if(!kill(cur->pid, SIGKILL)) {
            printf("Process [%d] killed\n", cur->pid);
        }
        free_node(cur);
    }
}