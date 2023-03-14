#ifndef _INPUTPARSE_H
#define _INPUTPARSE_H

/*
 * counts and returns the number of words 
 * in the given string
 */
int num_words(char *);

/*
 * split input into a char array of 
 * strings and returns a pointer to it
 */
char **tokenize(char *);

#endif