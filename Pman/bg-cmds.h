#ifndef _BGCMDS_H
#define _BGCMDS_H

/*
 * takes two strings and finds the max
 * string length and returns it
 */
size_t max(char *, char *);

/*
 * checks if command given is a starts with the 
 * "bg" and returns non zero int if true
 */
int is_bg(char *);

/*
 * checks string passeed in and returns an int
 * correlated with a specific bg command
    0 - non-bg command
	1 - bg
	2 - bglist
	3 - bgkill
	4 - bgstop
	5 - bgstart
 */
int bg_type(char *);

/* 
 * checks if file name passed in is in the 
 * current working directory, returns 1 if it is
 * 0 if it's not.
 * the idea for this function is from Tutorials Point:
 * https://www.tutorialspoint.com/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-cplusplus#:~:text=Call%20opendir()%20function%20to,name%20using%20en%2D%3Ed_name.
 */
int in_cwd(char *);

/*
 * runs the command passed by user with bg 
 * in front of it, adds pathname if it's in cwd 
 * otherwise assumes command to be built-in
 * returns 0 if command length is too short
 * else return 1
 */
int bg_cmd(char **, int , char *);

/*
 * changes state of given pid to the indicated state
 * 0 - kill, 1 - pause, 2 - start
 * if process doen't exist print error message
 */
void bg_statechange(char*, char *, int);

/*
 * prints out the process stats (ie. comm, state, utime,
 * stime, rss, voluntary context switches, and nonvoluntary 
 * context swtitches) from /proc. the idea about how to 
 * read /proc is from stackoverflow:
 * https://stackoverflow.com/questions/29991182/programmatically-read-all-the-processes-status-from-proc 
 */
void pstat(char *);

#endif