#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "linked-list.h"

extern node *jobs;

size_t max(char *a, char *b) {
	size_t len_a = strlen(a);
	size_t len_b = strlen(b);
	if(len_a > len_b) return len_a;
	return len_b;
}

int is_bg(char *cmd) {
	return !strncmp(cmd, "bg", 2);
}

int bg_type(char *cmd) {
	/*
	1 - bg
	2 - bglist
	3 - bgkill
	4 - bgstop
	5 - bgstart
	*/
	if(!is_bg(cmd)) {
		return 0;
	}
	if(!strncmp(cmd, "bg", max(cmd, "bg"))) { // bg ./* executable
		return 1;
	} else if (!strncmp(cmd, "bglist", max(cmd, "bglist"))) {
		return 2;
	} else if (!strncmp(cmd, "bgkill", max(cmd, "bgkill"))) {	
		return 3;
	} else if (!strncmp(cmd, "bgstop", max(cmd, "bgstop"))) {
		return 4;
	} else if (!strncmp(cmd, "bgstart", max(cmd, "bgstart"))) {
		return 5;
	}
	return 0;
}

int in_cwd(char *file) {
	DIR *dr;
    struct dirent *en;
    dr = opendir("."); //open all or present directory
	
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
    		if (!strncmp(en->d_name, file, max(en->d_name, file))) return 1;
        }
        closedir(dr); //close all directory
    }
	return 0;
}

int bg_cmd(char **input, int len, char *line) {
	if(len <= 1) return 0;
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	char name[256] = "";
	if(strncmp(input[1], "./", 2) && in_cwd(input[1])) {
		strcat(cwd, "/");
		strncat(cwd, input[1], strlen(input[1]));

		char *new_in1 = checked_malloc(strlen(input[1])+4);
		strncpy(new_in1, "./", 3);
		strcat(new_in1, input[1]);
		strncpy(input[1], new_in1, strlen(new_in1)+1);
		free(new_in1);

		strncpy(name, cwd, strlen(cwd)+1);
	} else if (!strncmp(input[1], "./", 2)) {
		strncpy(name, cwd, strlen(cwd)+1);
		strncat(name, (&input[1][1]), strlen(input[1]));
	} else {
		strncpy(name, input[1], strlen(input[1])+1);
	}
	
	pid_t pid = fork();

	if(pid < 0) {
		fprintf(stderr, "fork fail !!\n");
		exit(1);
	}
	if(pid == 0) { // child
		if(execvp(input[1], &input[1]) == -1) {
			printf("%s: command not found\n", line);
			exit(0);
		}
	} else {
		node *temp = new_node(name, pid);
		jobs = add_front(jobs, temp);
		usleep(90000);
		
		if(waitpid(pid, NULL, WNOHANG)) {
			jobs = remove_pid(jobs, pid);
		} else {
			printf("Process [%d] created\n", pid);
		}
	}
	return 1;
}

void bg_statechange(char* str_pid, char *line, int change) {
	// 0 - kill , 1 - pause, 2 - start //
	pid_t pid = atoi(str_pid);
	if(change == 0) {
		if(kill(pid, SIGKILL)) {
			printf("Error: Process %d does not exist\n", pid);
			return;
		}
		printf("Process [%d] killed\n", pid);
		jobs = remove_pid(jobs, pid);
	} else if (change == 1) {
		if(kill(pid, SIGSTOP)) {
			printf("Error: Process %d does not exist\n", pid);
			return;
		}
		change_state(jobs, pid, 0);
		printf("Process [%d] paused\n", pid);
	} else {
		if(kill(pid, SIGCONT)) {
			printf("Error: Process %d does not exist\n", pid);
			return;
		}
		change_state(jobs, pid, 1);
		printf("Process [%d] resumed\n", pid);
	}
		
}

void pstat(char *pid) {
	char filename[100];
	char buffer[100];
	pid_t unused;
	char comm[1000];
	char state;
	long unsigned int utime;
	long unsigned int stime;
	long int rss;
	unsigned int v_ctxt;
	unsigned int nv_ctxt;
	sprintf(filename, "/proc/%s/stat", pid);
	FILE *f = fopen(filename, "r");
	if(!f) {
		printf("Error: Process %s does not exist\n", pid);
		return;
	}	
	fscanf(f, "%d %s %c", &unused, comm, &state);
	for(int i = 0; i < 10; i++) {
		fscanf(f, "%d", &unused);
	}
	fscanf(f, "%lu %lu", &utime, &stime);
	for(int i = 0; i < 8; i++) {
		fscanf(f, "%d", &unused);
	}
	fscanf(f, "%ld", &rss);
	fclose(f);

	sprintf(filename, "/proc/%s/status", pid);
	f = fopen(filename, "r");
	if(!f) {
		printf("Error: Process %s does not exist\n", pid);
		return;
	}	
	while(fgets(buffer, 100, f)) {
        if(strncmp(buffer, "voluntary_ctxt_switches:", strlen("voluntary_ctxt_switches:")))
            continue;
        // Ignore "voluntary_ctxt_switches:" and whitespace
		sscanf(buffer, "%s %d", filename, &v_ctxt);
		fgets(buffer, 100, f);
		sscanf(buffer, "%s %d", filename, &nv_ctxt);		
        break;
    }
	fclose(f);

	printf("\n[%s]--------------------------------\n", pid);
	printf("comm: %s\n", comm);
	printf("state: %c\n", state);
	printf("utime: %.2f\n", utime/(float)sysconf(_SC_CLK_TCK));
	printf("stime: %.2f\n", stime/(float)sysconf(_SC_CLK_TCK));
	printf("rss: %ld\n", rss);
	printf("voluntary_ctxt_switches: %d\n", v_ctxt);
	printf("nonvoluntary_ctxt_switches: %d\n", nv_ctxt);
	printf("--------------------------------[%s]\n\n", pid);
}