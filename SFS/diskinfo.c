#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "linked-list.h"
#include "diskFunc.c"


// finds label of disk in the root directory by looking for the 
// volume label, if you're unable to find it copy over the label
// found in the boot sector
void label_on_disk(FILE *fp, char *label, boot_t boot_sector) {
    fseek(fp, 0x2600, SEEK_SET);
    //19-32
    entry_t root_entry;
    for(int i = 0; i < 224; i++) {
        fread(&root_entry, sizeof(root_entry), 1, fp); 
        if((root_entry.attributes & 0x8) != 0 && root_entry.attributes != 0xF) {
            strncpy(label, root_entry.filename, 11);
            return;
        }
    }
    strncpy(label, boot_sector.label, 11);
}

int countFiles(FILE *fp, int sectors);

// assistant function for a recursive file count function
// sets up file pointer
int start_countFiles(FILE *fp, int loc /*0x2600*/ , int sectors) {
    fseek(fp, loc, SEEK_SET);
    return countFiles(fp, sectors);
}

// counts all of the files at the given location for the 
// given number of sectors
int countFiles(FILE *fp, int sectors) {
    entry_t root_entry;
    int files = 0;
    node_t *head = NULL;
    for(int i = 0; i < sectors*512/32; i++) { //19-32
        fread(&root_entry, sizeof(root_entry), 1, fp); 
        if(root_entry.attributes == 0x0 && !emptyEntry(root_entry)) {
            files++;
        } else if (isDir(root_entry)) head = add_end(head, new_node(root_entry));
    }
    for(node_t *cur = head; cur != NULL; cur = cur->next) {
        files += start_countFiles(fp, (31+cur->entry.cluster)*512, 1);
    }
    free_list(head);
    return files;
}

int main(int argc, char* argv[]) {
    
    FILE *fp;
    if(argc == 1) {
        printf("run with image name\n");
        exit(0);
    }
    boot_t boot_sector;
    char label[11];

    fp = fopen(argv[1], "r");
    if(!fp) { printf("'%s' not found\n", argv[1]); exit(0); }

    fread(&boot_sector, sizeof(boot_sector), 1, fp);
    printf("OS Name: %.8s\n", boot_sector.name);

    label_on_disk(fp, label, boot_sector);
    printf("Label of the disk: %s\n", label);
    printf("Total size of disk: %d\n", boot_sector.total_sectors*boot_sector.bytes_per_sector);
    printf("Free size of disk: %d\n", getFreeSpace(fp));
    printf("Number of files on disk: %d\n", start_countFiles(fp, 0x2600, 14));
    printf("Number of FAT copies: %d\n", boot_sector.fats);
    printf("Sectors per FAT: %d\n", boot_sector.sectors_per_fat);

    fclose(fp);
    
} // main   