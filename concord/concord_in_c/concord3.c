/*
 * concord3.c
 * 
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2022.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "emalloc.h"
#include "seng265-list.h"

// reads input and depending on the number passed in
// 0 being exclusion
// 1 reads in phrases
node_t* read_input(int i) {
    size_t len = 0;
    char *buff = NULL;
    node_t* holder = NULL; // list to be returned
    node_t* to_be_added = NULL;
    
    // checks if input file is correct version
    if (i == 0) {
        getline(&buff, &len, stdin);
        if (buff[0] == '1') {
            printf("Input is version 1, concord3 expected version 2\n");
            free(buff);
            exit(0);
        } // if
        getline(&buff,&len,stdin); // remove the '' line
    } // if

    while (getline(&buff, &len, stdin) > 0){
        if (i == 0 && buff[0] == '\"') { break; } // end of exclusions
        char temp[strlen(buff)+1];
        strncpy(temp, buff, strlen(buff)+1);
        temp[strlen(temp)-1]  = '\0'; // replace \n with \0
        if (i == 0){ // if exclusion words change to lowercase
            for(int k = 0; k < strlen(temp); k++) 
	        temp[k] = tolower(temp[k]);
        } // if
        to_be_added = new_node(temp);
        holder = add_end(holder, to_be_added);
    } // while
    free(buff);
    return holder;
} // read_input

void free_list(node_t* head) {
    node_t* next_node = NULL;
    for(node_t* cur = head; cur != NULL; cur = next_node) {
        next_node = cur->next;
        free(cur);
    } // for
} // free_list

// tokenizes a char array into a list of words
node_t* tokenize(char* line) {
    char* t;
    node_t* list = NULL;
    t = strtok(line, " ");
    while (t) {
        list = add_end(list, new_node(t)); 
        t = strtok(NULL, " ");
    } // while
    return list;
} // tokenize

int is_exclusion(node_t* ex_list, char* word) {
    char lower_word[strlen(word)+1];
    int i;
    // copy word to be compared in lowercase
    for (i = 0; i < strlen(word); i++) {
        lower_word[i] = tolower(word[i]);
    } // for
    lower_word[i] = '\0';
    
    for (node_t* cur = ex_list; cur != NULL; cur = cur->next){
        if (strncmp(cur->text, lower_word, 1+ strlen(cur->text)>strlen(lower_word)? strlen(cur->text):strlen(lower_word)) == 0){
            return 1;
        } // if
    } // for
    return 0;
} // is_exclusion

int index_of_key(char* line, char* key) {
    return (int)(strstr(line, key)-line);
} // index_of_key

node_t* add_to_entries(node_t* add_to, node_t* line, char* key) {
    char text[LIST_MAXLEN_TEXT] = "\0";
    char* sep = "";
    char key_upper[1+strlen(key)];
    
    // change key to be inserted to uppercase
    for (int i = 0; i <= strlen(key); i++) {
        key_upper[i] = toupper(key[i]);
    } // for
    // for each word in the line 
    for (node_t* cur = line; cur!= NULL; cur = cur->next) {
        strncat(text,sep,1);
        int len = strlen(cur->text)>strlen(key) ? strlen(cur->text):strlen(key);
        if (strncmp(key, cur->text,1+len) == 0) {
            strncat(text,key_upper, strlen(key_upper));
        } else {
            strncat(text, cur->text, strlen(cur->text));
        } // else
        sep = " ";
    } // for
    node_t* to_be_added = new_node_wkey(text, key_upper,index_of_key(text, key_upper));
    add_to = add_ordered_wkey(add_to, to_be_added);
    return add_to;
} // add_to_entries

// check which words are exclusions and find keys
node_t* check_n_find(node_t* phr, node_t* ex) {
    node_t* sorted_list = NULL;
    char *line;
    node_t* splitline = NULL;

    for (node_t* cur = phr; cur != NULL; cur = cur->next) {
        line = (char *)malloc(sizeof(char)*(strlen(cur->text)+1));
        if (line == NULL) { printf("Malloc failed for char* line in check_n_find()"); exit(0); }
        strncpy(line, cur->text, (strlen(cur->text)+1));
        splitline = tokenize(line);
        for (node_t* word = splitline; word != NULL; word = word->next){
            if (!is_exclusion(ex, word->text))
                sorted_list = add_to_entries(sorted_list, splitline, word->text);
        } // for
        free(line);
        free_list(splitline);
    } // for
    return sorted_list;
} // check_n_find

void remove_first_word(char* line) {
    char new[strlen(line)];
    int removed = 0;
    int new_idx = 0;
    for(int i = 0; i < strlen(line); i++) {
        if(line[i] == ' ' && removed == 0){
            removed = 1;
            continue;
        } else if (removed == 0) {
            continue;
        } // else if
        new[new_idx++] = line[i];
    } // for
    new[new_idx] = '\0';
    strncpy(line, new, 1+strlen(new));
} // remove_first_word

void remove_last_word(char* line) {
    for(int i = strlen(line); i >= 0; i--) {
        if(line[i] == ' ') {
            line[i] = '\0';
            return;
        } // if
    } // for
} // remove_last_word

// removes any overflow words from output
node_t* format_output(node_t* list) {
    for(node_t* cur = list; cur != NULL; cur = cur->next) {
        while(index_of_key(cur->text,cur->key) > 20) {
            remove_first_word(cur->text);
        } // while
        while(strlen(cur->text) - index_of_key(cur->text,cur->key) > 31) {
            remove_last_word(cur->text);
        } // while
        cur->key_i = index_of_key(cur->text,cur->key);
    } // for
    return list;
} // format_output

/* outputs node to be correctly formated */
void output_node(node_t* n) {
    for(int i = 0; i < 29 - n->key_i; i++)
        printf(" ");
    printf("%s\n",n->text);
} // output_node

int main(int arc, char *argv[]) {
    node_t *ex_head = read_input(0);
    node_t *phr_head = read_input(1);
    node_t* sorted = check_n_find(phr_head, ex_head);
    free_list(phr_head);
    free_list(ex_head);
    sorted = format_output(sorted);

    for(node_t *cur = sorted; cur != NULL; cur = cur->next){
        output_node(cur);
    } // for
    free_list(sorted);
} // main
