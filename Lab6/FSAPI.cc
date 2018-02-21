#include "FSAPI.h"

char inputName[30];

const i_index_t HOMEINODE = 0;

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
    current.i = HOMEINODE;
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
    path workPath;
    // path 
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("path error\n", stderr);
        return -1;
    }
    if (!strcmp(decodePath.content[0], "/")) {  // absolute path
        workPath.i = HOMEINODE;
        workPath.p = 1;
        strcpy(workPath.content[0], "/");
        decodePath.i = 1;
    }
    else {
        workPath = current; // related path
    }
    for (int index = decodePath.i; index < decodePath.p; ++index) {
        i_index_t find_i;
        uint16_t pos;
        if (s_search(workPath.i, decodePath.content[index], &find_i, &pos) == -1) {
            fputs("no such a directory\n", stderr);
            return -1;
        }
        if (i_nodes[find_i].i_type != DIRTYPE) {
            fputs("not a directory\n", stderr);
            return -1;
        }
        s_changedir(&workPath, decodePath.content[index], find_i);
    }
    current = workPath;
    return 0;
}