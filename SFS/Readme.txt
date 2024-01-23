Assignment 3 (CSC 360)
Date: April 4th, 2023
VNum: V00986656
Name: Sara Subedi

To run my program type "make" then:
    Part I   - type "./diskinfo <disk image>"
        Provides basic info of the disk image that is inputed.
    Part II  - type "./disklist <disk image>"
        Prints all of the directories found on the disk. Starts at the root.
    Part III - type "./diskget <disk image> <filename>"
        Outputs the desired file from the disk into the current directory.
    Part IV  - type "./diskput <disk image> [destination path] <filename>"
        Takes the given file and puts it into the directory, is destionation path isn't provided 
        the file is put into the root. The destination path should be in the format 
        "dirname/subdirname" or "dirname/subdirname/"