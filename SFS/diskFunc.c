/*
 * Function taken from CSC 360 One Drive
 */
int getFreeSpace(FILE * fp) {
    int i, free_bytes = 0;
    uint8_t X0, X1, X2;
    uint16_t X, Y;
    fat_entry_t fat_entry;

    fseek(fp, 0x200, SEEK_SET);
    for(i=0; i<768; i++) { // 9 * 512 / 3
        fread(&fat_entry, sizeof(fat_entry), 1, fp);
        X0 = fat_entry.b0;
        X1 = fat_entry.b1;
        X2 = fat_entry.b2;

        X = ((X1 & 0x0F) << 8) + X0;
        Y = ((X1 & 0xF0) >> 4) + (X2 << 4);

        if (X == 0) {
            free_bytes += 1;
        } 
        if (Y == 0) {
            free_bytes += 1;
        } 
    }
    return free_bytes*512;
}

// check the state of the given cluster in the fat table
// if it is the last cluster returns 0
// if it is points to the next cluster return cluster
// if unexpected value return -1 (error)
int checkFAT(FILE * fp, int cluster) {
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
        if(Y >= 0xFF8 && Y<= 0xFFF) { // last cluster
            return 0;
        } else if (Y != 0 && Y != 0xFF7 && !(Y >= 0xFF0 && Y <= 0xFF6)) {
            return Y;
        } else {
            return -1;
        }
    } else {
        if(X >= 0xFF8 && X<= 0xFFF) {
            return 0;
        } else if (X != 0 && X != 0xFF7 && !(X >= 0xFF0 && X <= 0xFF6)) {
            return X;
        } else {
            return -1;
        }
    }
}

// checks if given entry is empty
int emptyEntry(entry_t entry) {
    return (entry.create_date == 0 && entry.create_time == 0 && entry.size == 0 && entry.last_access_date == 0);
}

// checks if given entry is a directory
int isDir(entry_t check) {
    return (check.attributes & 0x10) && (strncmp(check.filename, ".", 1));
}