#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "linked-list.h"
#include "emalloc.h"
#include "diskFunc.c"
#include "strparse.c"

int cluster; // cluster where the file contents are located
int size; // size of file to be copied 

// finds the file within the root directory and saves
// file size and first cluster location to the cooresponding 
// global variables
void findFile(FILE *fp, char *filename) {
    fseek(fp, 0x2600, SEEK_SET);
    entry_t root_entry;
    for(int i = 0; i < 14*512/32; i++) { //goes through clusters 19-32
        fread(&root_entry, sizeof(root_entry), 1, fp);
        if(root_entry.attributes == 0x0 && !emptyEntry(root_entry)) {
            char name[12];
            int idx = firstSpace(root_entry.filename, 8);
            strncpy(name, root_entry.filename, idx);
            name[idx] = '\0';
            strncat(name, ".", 2);
            strncat(name, root_entry.extension, firstSpace(root_entry.extension, 3));
            if(!strcmp(name, filename)) {
                cluster = root_entry.cluster;
                size = root_entry.size;
                return;
            }
        }
    }
    printf("File not found\n"); 
    exit(0);
}

int main(int argc, char* argv[]) {
    
    FILE *fp; // file pointer of image disk
    if (argc != 3) {
        printf("Incorrect number of arguments: ./diskget <disk image> <filename>\n");
        exit(0);
    }

    fp = fopen(argv[1], "r");
    if(!fp) { printf("'%s' not found\n", argv[1]); exit(0); }

    boot_t boot_sector;
    fread(&boot_sector, sizeof(boot_sector), 1, fp);

    string_toUpper(argv[2], strlen(argv[2]));
    findFile(fp, argv[2]);
    FILE *writeto = fopen(argv[2], "wb");

    int peak = checkFAT(fp, cluster);
    // copies the desired files cluster by cluster and 
    // puts them into cooresponding output file of same name
    for(int i = cluster; i != -1; i = peak, peak = checkFAT(fp, peak)) {
        if (i == 0) { // last cluster
            break;
        } else if (i == -1) {
            printf("Error\n");
            exit(0);
        }
        file_t temp;
        fseek(fp, 512*(31+i), SEEK_SET);
        fread(&temp, sizeof(temp), 1, fp);

        int copysize;
        if(peak == 0) { // if copying last cluster
            copysize = size%512;
        } else {
            copysize = sizeof(temp);
        }
        fwrite(&temp, copysize, 1, writeto);
    }
    fclose(writeto);
    fclose(fp);
    
} // main   