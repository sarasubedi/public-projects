#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "linked-list.h"
#include "diskFunc.c"
#include "strparse.c"

// prints out the date using the given
// 16 bit integer 
void printFormatDay(uint16_t date) {
    uint32_t day = date & 0x1F;
    uint32_t month = date & 0b111100000;
    month = (month >> 5);
    uint32_t year = date & 0b1111111000000000;
    year = (year >> 9);
    printf("%.4d-%.2d-%.2d ", year+1980, month, day);
}

// prints out the time using the given
// 16 bit integer 
void printFormatTime(uint16_t time) {
    uint32_t minutes = time & 0b11111100000;
    minutes = (minutes >> 5);
    uint32_t hours = time & 0b1111110000000000;
    hours = (hours >> 11);
    printf("%.2d:%.2d\n", hours, minutes);
}

// prints out the given entry according to the
// specified format
void printEntry(entry_t entry){
    char name[12];
    if(isDir(entry)) {
        printf("D ");
        strncpy(name, entry.filename, 11);
    } else {
        printf("F ");
        int idx = firstSpace(entry.filename, 8);
        strncpy(name, entry.filename, idx);
        name[idx] = '\0';
        strncat(name, ".", 2);
        strncat(name, entry.extension, firstSpace(entry.extension, 3));
    }
    printf("%d", entry.size);
    if(entry.size) printSpaces(10-log10(entry.size)+1);
    else printSpaces(10);
    printf("%.12s", name);
    printSpaces(21 - strlen(name));
    printFormatDay(entry.create_date);
    printFormatTime(entry.create_time);
}

void start_printDir(FILE *fp, char *dirName, int loc, int sectors);

// prints every entry within the given sector
// then recursivly calls any directories that 
// were within the current dirrectory
void printDir(FILE *fp, int curLoc, int sectors) {
    entry_t root_entry;
    node_t *head = NULL;
    for(int i = 0; i < sectors*512/32; i++) {
        fread(&root_entry, sizeof(root_entry), 1, fp); 
        if(root_entry.attributes != 0xF && (!emptyEntry(root_entry) || isDir(root_entry))) {
            printEntry(root_entry);
            if(isDir(root_entry)) { head = add_end(head, new_node(root_entry)); }
        }
        if(i == sectors*512/32-1 && curLoc != 0x2600) { 
            // if at the end of the dir check if there is 
            // another sector to traverse
            int fatVal = checkFAT(fp, curLoc/512 - 31);
            if(fatVal != 0){
                i = 0;
                curLoc = (fatVal+31)*512;
                fseek(fp, curLoc, SEEK_SET);
                continue;
            }   
        }
    }
    for(node_t *cur = head; cur != NULL; cur = cur->next) {
        start_printDir(fp, cur->entry.filename, (31+cur->entry.cluster)*512, 1);
    }
    free_list(head);
}

// prints current directory name and sets up file pointer
// to be used in printDir()
void start_printDir(FILE *fp, char *dirName, int loc, int sectors) {
    printf("\n%s\n", dirName);
    printf("==================\n");
    fseek(fp, loc, SEEK_SET);
    printDir(fp, loc, sectors);
}

int main(int argc, char* argv[]) {
    
    FILE *fp;
    if(argc == 1) {
        printf("run with image name\n");
        exit(0);
    }
    fp = fopen(argv[1], "r");
    if(!fp) { printf("'%s' not found\n", argv[1]); exit(0); }

    start_printDir(fp, "ROOT", 0x2600, 14);
    printf("\n");

    fclose(fp);
    
} // main   