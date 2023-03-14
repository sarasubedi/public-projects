#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked-list.h"

int num_words(char *line) {
	if(strlen(line) <= 0) return 0;
	int last_space = 1;
	int count = 0;
	for(int i = 0; line[i]; i++) {
		if((last_space && line[i] == ' ') || line[i] == ' ') { 
			last_space = 1;
			continue;
		} else if(last_space && line[i]!=' ') {
			count++;
			last_space = 0;
		}
	}
	return count;
}

char **tokenize(char *input) {
	char **line = (char **)checked_malloc(sizeof(char*)*(num_words(input)+1+3));
	int idx = 0;
	char *t;
	
	t = strtok(input, " ");
	while(t) {
		line[idx] = (char *)checked_malloc(sizeof(char)*(strlen(t)+1));
		strncpy(line[idx++], t, strlen(t)+1);
		t = strtok(NULL, " ");
	}
	line[idx] = NULL;
	return line;
}