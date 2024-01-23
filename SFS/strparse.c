#include <ctype.h>

// finds first space in a string
// if there are no spaces returns string length
int firstSpace(char* name, int len) {
    for(int i = 0; i < len; i++) {
        if(name[i] == ' ') return i;
    }
    return len;
}

// fills in the string with spaces from the
// null terminator to the provided int
void fillSpaces(char *str, int fill) {
    int i = 0;
    while(str[i]) i++;
    while(i < fill) {
        str[i] = ' ';
        i++;
    }
}

// find the first '/' character to apear in string
// and return index, returns -1 if no '/'
int findSlash(char* str, int len) {
    int i = 0;
    while(i < len) {
        if(str[i] == '/') return i;
        i++;
    }
    return -1;
}

// finds '.' character in a filename
int findDecimal(char *str) {
    int i = 0;
    while(str[i]!= '.' && str[i]) i++;
    return i;
}

// prints the number of spaces desired
void printSpaces(int i) {
    for(int j = 0; j < i; j++) printf(" ");
}

// changes given string into uppercase
void string_toUpper(char *str, int len) {
    int i = 0;
    while(i < len) {
        str[i] = toupper(str[i]);
        i++;
    }
}

// returns maximum value of the
// passed in integers
int max(int i, int j) {
    if (i>j) return i;
    return j;
}