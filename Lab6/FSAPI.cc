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
    strcpy(current.content[current.p++], "/");
    current.i = i;
    return 0;
}

int fs_load() {
    if (openFp() == -1) {
        fputs("can not load file system\n", stderr);
        fputs("miss the dev file??\n", stderr);
        return -1;
    }
    strcpy(current.content[current.p++], "/");
    current.i = 0;
    return 0;
}

char * fs_pwd() {
    if (current.p == 1) {
        char *buffer = (char *)malloc(sizeof(char) * 2);
        strcpy(buffer, "/");
        return buffer;
    }
    uint16_t sumLength = 1;
    for (int i = 1; i < current.p; ++i) {
        sumLength += strlen(current.content[i]) + 1;
    }
    char * buffer = (char *)malloc(sizeof(char) * sumLength);
    int pos = 0;
    for (int i = 1; i < current.p; ++i) {
        strcpy(buffer + pos, "/");
        strcpy(buffer + pos + 1, current.content[i]);
        pos += strlen(current.content[i]) + 1;
    }
    return buffer;
}

int fs_cd(char *p) {
    path decodePath;
    // path 
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("path error\n", stderr);
        return -1;
    }
    if (!strcmp(decodePath.content[0], "/")) {  // absolute path

    }
}