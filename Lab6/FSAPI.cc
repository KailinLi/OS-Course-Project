#include "FSAPI.h"

char inputName[30];

int fs_init() {
    printf("Making dev...\n");
    if (initSpace() == -1)
        return -1;
    printf("Opening dev...\n");
    if (openFp() == -1)
        return -1;
    printf("Making root...\n");
    i_index_t i;
    b_index_t b;
    if (s_newNode(&i, &b) == -1) {
        fputs("root error\n", stderr);
        return -1;
    }
    strcpy(inputName, ".");
    if (s_addEntry(i, inputName, i) == -1) {
        fputs("error\n", stderr);
        return -1;
    }
    strcpy(inputName, "..");
    if (s_addEntry(i, inputName, i) == -1) {
        fputs("error\n", stderr);
        return -1;
    }
    strcpy(current, "/");
    current_i = i;
    return 0;
}

char * fs_pwd() {
    char * buffer = (char *)malloc(sizeof(char) * (strlen(current) + 1));
    strcpy(buffer, current);
    return buffer;
}