#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>

#include "linked-list.h"
#include "input-parse.h"
#include "bg-cmds.h"

node *jobs = NULL; // list of currently running bg processes

int main (int argc, char* argv[]) {
	pid_t pid;

	// in loop while PMan accepts input
	while(1) {
		usleep(3000);
		char* line = readline( "PMan: > ");
		usleep(3000);
		if (strlen(line) <= 0) continue; // ignore if nothing entered
		
		char *cpy = (char *)checked_malloc(sizeof(char)*(strlen(line)+1));
		strncpy(cpy, line, strlen(line)+1);
		int len = num_words(cpy);
		
		char **input = tokenize(cpy);
		free(cpy);

		// if exit command entered kill all jobs that were started and exit program
		if (len == 1 && !strncmp(input[0], "exit", max(input[0], "exit"))) {
			jobs = update_list(jobs);
			free(line);
			if(jobs) free_list(jobs);
			for(int i = 0; i < num_words(line); i++) free(input[i]);
			free(input);
			break;
		}
		
		int type = bg_type(input[0]);

		if(type) { // "bg" function
			switch(type) { // based on cmd inputed
				case 1: if(bg_cmd(input, len, line)) break;
						printf("%s: command not found\n", line);
						break;
				case 2: if(len == 1) { jobs = update_list(jobs); print_list(jobs); break; }
						printf("%s: command not found\n", line);
						break;
				case 3: if(len == 2) { bg_statechange(input[1], line, 0); break; }
						printf("%s: command not found\n", line);
						break;
				case 4: if(len == 2) { bg_statechange(input[1], line, 1); break; }
						printf("%s: command not found\n", line);
						break;
				case 5: if(len == 2) { bg_statechange(input[1], line, 2); break; }
						printf("%s: command not found\n", line);
						break;
			}
		} else if(!strncmp(input[0], "pstat", max("pstat", input[0]))) {
			if(len != 2) {
				printf("%s: command not found\n", line);
			} else {
				pstat(input[1]);
			}
		} else { // normal command line functiton
			pid = fork();
			if(pid < 0) {
				fprintf(stderr, "fork fail !!\n");
				return 1;
			}else if(pid == 0) { // child
				if(execvp(input[0], input) == -1) {
					printf("%s: command not found\n", line);
					return 1;
				}
			} else { // parent waits for foreground process
				wait(NULL);
			}
		}
		// free mallocs and add typed command to history
		for(int i = 0; i < num_words(line)+1; i++) free(input[i]);
		free(input);
		add_history(line);
		free(line);
	}		
}