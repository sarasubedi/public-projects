Date: February 5th 2023
Name: Sara Subedi

This program runs in a way that is similar to a terminal. It can execute commands like ls, pwd, and ps. 
It can run programs in the background with the command "bg ./inf" 
You can:
- look at the background programs with the command "bglist"
- kill a process by typing "bgkill pid"
- pause a process by typing "bgstop pid"
- play a paused process by typing "bgstart pid"
- all of the processes details from \proc can be viewed by tying "pstat pid"


Descriptions of each function can be found in each .c files respective .h file.
More specific comments can be found in the functions themselves.

The program kills any processes that were created within PMan and exits itself
when "exit" is entered into the terminal.

The user is told if processes were killed (using "kill") or exited in the next "bglist" call
following. "bgkill" tells the user immediately after the fact.

How to compile & run:
    - type "make" to compile and "./PMan" to run
    - type "exit" while in PMan to end program
    - to remove the object files and PMan executable type "make clean"

Files:
    - PMan.c file containing main
    - Makefile compiles all of the code
    - linked-list.c/h sets up node struct and basic linked-list functions
    - input-parse.c/h functions to simplify reading user input 
    - bg-cmds.c/h functions related to the processes running in the background