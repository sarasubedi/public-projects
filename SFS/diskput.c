#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "linked-list.h"
#include "emalloc.h"
#include "diskFunc.c"
#include "strparse.c"

entry_t tobeInserted; 
node_t *freeClusters = NULL; // list of the needed amount of free clusters
node_t *dirlist = NULL; // list of directories that file will go into

// updates both fat tables of the current cluster with the next cluster
// that data will be put into
void updateFAT(FILE * fp, int cluster, int newVal) {
    uint8_t X0, X1, X2;
    uint16_t X, Y;
    fat_entry_t fat_entry;
    int offset = (cluster&1) ? (cluster-1)*3/2:cluster*3/2;

    fseek(fp, 0x200 + offset, SEEK_SET);
    fread(&fat_entry, sizeof(fat_entry), 1, fp);
    X0 = fat_entry.b0;
    X1 = fat_entry.b1;
    X2 = fat_entry.b2;

    X = ((X1 & 0x0F) << 8) + X0;
    Y = ((X1 & 0xF0) >> 4) + (X2 << 4);
    
    if(cluster&1) {
        if(!newVal) { // if EOF
            Y = 0xFFF;
        } else Y = newVal;
    } else {
        if(!newVal) { // if EOF
            X = 0xFFF;
        } else X = newVal;
    }
    X0 = X & 0x0FF;
    X1 = ((X & 0xF00) >> 8) | ((Y & 0x00F) << 4);
    X2 = ((Y & 0xFF0) >> 4);

    fat_entry.b0 = X0;
    fat_entry.b1 = X1;
    fat_entry.b2 = X2;
    fseek(fp, -sizeof(fat_entry), SEEK_CUR); // backtrack to put in updated entry
    fwrite(&fat_entry, sizeof(fat_entry), 1, fp);
    fseek(fp, 5120 + offset, SEEK_SET);
    fwrite(&fat_entry, sizeof(fat_entry), 1, fp);

}

// finds the desired number of free clusters and saves them 
// in the global list freeClusters
void findFreeFAT(FILE * fp, int numClusters) {
    uint8_t X0, X1, X2;
    uint16_t X, Y;
    fat_entry_t fat_entry;
    entry_t temp;
    int count = 0;

    fseek(fp, 0x200, SEEK_SET);
    for(int i=0; i<768 && count < numClusters; i++) { // 9 (sectors) * 512 (bytes per) / 3 (bytes read together)
        fread(&fat_entry, sizeof(fat_entry), 1, fp);
        X0 = fat_entry.b0;
        X1 = fat_entry.b1;
        X2 = fat_entry.b2;

        X = ((X1 & 0x0F) << 8) + X0;
        Y = ((X1 & 0xF0) >> 4) + (X2 << 4);
        
        if (X == 0 && count < numClusters) {
            temp.cluster = i*2;
            freeClusters = add_end(freeClusters, new_node(temp));
            count++;
        } 
        if (Y == 0 && count < numClusters) {
            temp.cluster = i*2+1;
            freeClusters = add_end(freeClusters, new_node(temp));
            count++;
        } 
    }
}

int start_findDir(FILE *fp, int loc, int sectors);

// tries to finds the directory in the first node of dirlist in the 
// specified number of sectors, returns the cluster the directory
// can be found in, if found
int findDir(FILE *fp, int sectors) {
    entry_t root_entry;
    for(int i = 0; i < sectors*512/32; i++) {
        fread(&root_entry, sizeof(root_entry), 1, fp); 
        if(root_entry.attributes != 0xF && isDir(root_entry)) {
            int len = max(strlen(dirlist->entry.filename), firstSpace(root_entry.filename, 8));
            if(!strncmp(dirlist->entry.filename, root_entry.filename, len)) {
                if(dirlist->next != NULL) {
                    node_t* temp = dirlist;
                    dirlist = dirlist->next;
                    free(temp);
                    return start_findDir(fp, (31+root_entry.cluster)*512, 1);
                } else {
                    return 31+root_entry.cluster;
                }
            }
        }
    }
    free_list(dirlist);
    return -1;
}

// sets up file pointer for findDir, if no directory provided
// returns sector of root directory
int start_findDir(FILE *fp, int loc, int sectors) { 
    fseek(fp, loc, SEEK_SET);
    if(dirlist == NULL)  return 19; 
    return findDir(fp, sectors);
}

// encodes date to match FAT12 standard
uint16_t encodeDate(struct tm day) {
    uint16_t date = day.tm_mday;
    uint16_t month = day.tm_mon + 1;
    month = (month << 5);
    uint16_t year = day.tm_year + 1900 - 1980;
    year = (year << 9);
    date = date | month | year;
    return date;
}

// encodes time to match FAT12 standard
uint16_t encodeTime(struct tm day) {
    uint16_t time = day.tm_sec;
    uint16_t min = day.tm_min;
    min = (min << 5);
    uint16_t hour = day.tm_hour;
    hour = (hour << 11);
    time = hour | min | time;
    return time;
}

// insert the entry into the first free space within numClusters
// starting from the provided cluster
void insertEntry(FILE *fp, int cluster, int numClusters) {
    fseek(fp, cluster*512, SEEK_SET);
    entry_t root_entry;
    for(int i = 0; i < numClusters*512/32; i++) { //19-32
        fread(&root_entry, sizeof(root_entry), 1, fp);
        if(emptyEntry(root_entry) && !isDir(root_entry)) {
            fseek(fp, -sizeof(tobeInserted), SEEK_CUR);
            fwrite(&tobeInserted, sizeof(tobeInserted), 1, fp);
            return;
        }
    }
}

// takes the inputed directory list and 
// puts each individual directory into the dirlist
void listDirs(char *dirstr) {
    entry_t curDir;
    int slash = -1;
    int i = 0;
    while(1){
        i = slash + 1;
        slash = findSlash(&dirstr[i], strlen(dirstr)-slash+1);
        if(slash == -1) {
            slash = strlen(dirstr);
            strcpy(curDir.filename, &dirstr[i]);
            node_t *temp = new_node(curDir);
            if(strlen(temp->entry.filename) == 0) { 
                // for the case where user inputs dirstr ending with '/'
                free(temp);
            } else {
                dirlist = add_end(dirlist, temp);
            }
            return;
        } else {
            slash += i; 
        }
        strncpy(curDir.filename, &dirstr[i], slash-i);
        curDir.filename[slash-i] = '\0';
        node_t *temp = new_node(curDir);
        dirlist = add_end(dirlist, temp);
    }
}

// fill in information about the file to be insterted
void init_tobeInserted(char *filename, struct stat sb) {
    strncpy(tobeInserted.filename, filename, findDecimal(filename));
    fillSpaces(tobeInserted.filename, 8);
    string_toUpper(tobeInserted.filename, 8);
    strncpy(tobeInserted.extension, &filename[findDecimal(filename)+1], 3);
    string_toUpper(tobeInserted.extension, 3);
    fillSpaces(tobeInserted.extension, 3);
    tobeInserted.attributes = 0;
    tobeInserted.create_date = encodeDate(*localtime(&sb.st_mtime));
    tobeInserted.create_time = encodeTime(*localtime(&sb.st_mtime));
    tobeInserted.last_access_date = encodeDate(*localtime(&sb.st_atime));
    tobeInserted.last_modified_time = tobeInserted.create_time;
    tobeInserted.last_modified_date = tobeInserted.create_date;
    tobeInserted.cluster = freeClusters->entry.cluster;
    tobeInserted.size = sb.st_size;
}

// insert the file into the filesystem. updates the fat tables and 
// writes the file at the same time
void insert_tobeInserted(FILE *image, FILE *fp) {
    node_t *cur;
    file_t data;
    for(cur = freeClusters; cur->next != NULL; cur = cur->next) {
        int curCluster = cur->entry.cluster;
        updateFAT(image, cur->entry.cluster, cur->next->entry.cluster);
        fseek(image, (curCluster+31)*512, SEEK_SET);
        fread(&data, sizeof(data), 1, fp);
        fwrite(&data, sizeof(data), 1, image);
    }
    updateFAT(image, cur->entry.cluster, 0);
    fseek(image, (cur->entry.cluster+31)*512, SEEK_SET);
    fread(&data, sizeof(data), 1, fp);
    fwrite(&data, tobeInserted.size%512, 1, image);
} 

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("Not enough args: ./diskput <disk image> [destination path] <filename>\n");
        exit(0);
    } else if (argc > 4) {
        printf("Too many args: ./diskput <disk image> [destination path] <filename>\n");
    }

    FILE *fp;
    char *filename;
    char *dir = NULL;

    // set up pointers according to whether a dest path was provided
    if(argc == 3) {
        filename = argv[2]; 
    } else {
        filename = argv[3];
        dir = argv[2];
        string_toUpper(dir, strlen(dir));
    }

    fp = fopen(filename, "rb"); // file to be written
    if(!fp) {
        printf("File not found.\n");
        exit(0);
    }

    struct stat sb;
    if (stat(filename, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);

    }
    // check if filename provided is a file
    if(S_ISREG(sb.st_mode) == 0) { 
        printf("Filename provided was a directory.\n");
        fclose(fp);
        exit(0);
    }

    FILE *image = fopen(argv[1], "r+");
    if(!image) {
        printf("Image file not found.\n");
        fclose(fp);
        exit(0);
    }

    // check if there is enough space for the file
    if(sb.st_size > getFreeSpace(image)) {
        printf("Not enough free space in the disk image.\n");
        fclose(fp);
        free(image);
        exit(0);
    }

    // if destination path provided collect directory list
    if(dir) { 
        listDirs(dir);
    }

    int cluster = start_findDir(image, 0x2600, 14);
    if(cluster == -1) {
        printf("The directory not found.\n");
        fclose(fp);
        fclose(image);
        exit(0);
    }
    
    // number of clusters needed to write file
    int numClusters = tobeInserted.size/512 +1;
    findFreeFAT(image, numClusters); 
    init_tobeInserted(filename, sb);
    
    // depending on destination different number of 
    // clusters to insert entry into
    if(dir == NULL) insertEntry(image, cluster, 14);
    else insertEntry(image, cluster, 1);
    insert_tobeInserted(image, fp);

    free_list(freeClusters);
    free_list(dirlist);
    fclose(fp);
    fclose(image);
} // main   