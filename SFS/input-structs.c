/*
    Structs given by Ahmad Abdullah in CSC 360
*/

typedef struct {
    char _a[3];
    char name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fats;
    uint16_t root_entries;
    uint16_t total_sectors; 
    uint8_t media_descriptor; 
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors2;
    uint8_t drive_index;
    uint8_t _b;
    uint8_t signature;
    uint32_t id;
    char     label[11];
    char     type[8];
    uint8_t _c[448];
    uint16_t sig; 
} __attribute__ ((packed)) boot_t;

typedef struct {
    char filename[8];
    char extension[3];
    uint8_t attributes;
    uint8_t _a;
    uint8_t create_time_us;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint8_t _b[2];
    uint16_t last_modified_time;
    uint16_t last_modified_date;
    uint16_t cluster;
    uint32_t size;
} __attribute__ ((packed)) entry_t;

typedef struct {
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
} __attribute__ ((packed)) fat_entry_t;

typedef struct {
    char data[512];
} __attribute__ ((packed)) file_t;