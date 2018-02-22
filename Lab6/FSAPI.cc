#include "FSAPI.h"

char inputName[30];

const i_index_t HOMEINODE = 0;

char uid_name[20][30];
char gid_name[20][30];
char host_name[30];

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
    for (int k = 0; k < 3; ++k)
        i_nodes[i].i_right[k] = default_dir_right[k];
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
    strcpy(host_name, "Codedo");
    strcpy(uid_name[0], "root");
    strcpy(gid_name[0], "wheel");
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

int fs_decode(path * decodePath, path * workPath, int pos) {
    if (!strcmp((*decodePath).content[0], "/")) {  // absolute path
        (*workPath).i = HOMEINODE;
        (*workPath).p = 1;
        strcpy((*workPath).content[0], "/");
        (*decodePath).i = 1;
    }
    else {
        *workPath = current; // related path
    }
    for (int index = (*decodePath).i; index < (*decodePath).p - pos; ++index) {
        i_index_t find_i;
        uint16_t pos;
        if (s_search((*workPath).i, (*decodePath).content[index], &find_i, &pos) == -1) {
            fputs("no such a directory\n", stderr);
            return -1;
        }
        if (i_nodes[find_i].i_type != DIRTYPE) {
            fputs("not a directory\n", stderr);
            return -1;
        }
        s_changedir(workPath, (*decodePath).content[index], find_i);
    }
    return 0;
}

int fs_divide(char * input, char (* param)[1000], size_t * length) {
    int pos = 0;
    int before_pos = 0;
    * length = 0;
    if (strlen(input) == 0) {
        fputs("no input\n", stderr);
        return -1;
    }
    int size = strlen(input);
    while (pos < size) {
        while (input[pos] != '\n' && input[pos] != ' ')
            ++pos;
        while (input[pos + 1] == ' ')
            ++pos;
        input[pos] = '\0';
        strcpy(param[(*length)++], input + before_pos);
        before_pos = ++pos;
    }
    return 0;
}

int fs_cd(char *p) {
    path decodePath;
    path workPath;
    // path 
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("path error\n", stderr);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 0) == -1)
        return -1;
    current = workPath;
    return 0;
}

int fs_ls(char * p, uint8_t mask) {
    path decodePath;
    path workPath;
    d_entry *entry;
    int length;
    if (s_handlepath(&decodePath, p) == -1) {
        entry = s_ls(current.i, &length);
    }
    else {
        if (fs_decode(&decodePath, &workPath, 0) == -1)
            return -1;
        entry = s_ls(workPath.i, &length);
    }
    if (mask & 1) { // -l
        int i;
        if (mask & (1 << 1))    // -a
            i = 0;
        else
            i = 2;
        for (; i < length; ++i) {
            printf("%s", (i_nodes[entry[i].i].i_type == DIRTYPE) ? "d" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[0] & (1 << 2)) ? "r" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[0] & (1 << 1)) ? "w" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[0] & 1) ? "x" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[1] & (1 << 2)) ? "r" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[1] & (1 << 1)) ? "w" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[1] & 1) ? "x" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[2] & (1 << 2)) ? "r" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[2] & (1 << 1)) ? "w" : "-");
            printf("%s", (i_nodes[entry[i].i].i_right[2] & 1) ? "x " : "- ");
            printf("%2d ", i_nodes[entry[i].i].i_nlink);
            printf("%8s ", uid_name[i_nodes[entry[i].i].i_uid]);
            printf("%8s ", gid_name[i_nodes[entry[i].i].i_gid]);
            printf("%5d ", i_nodes[entry[i].i].i_size);
            if (i_nodes[entry[i].i].i_type == DIRTYPE)
                printf(BLU);
            printf("%s\n", entry[i].name);
            if (i_nodes[entry[i].i].i_type == DIRTYPE)
                printf(RESET);
        }
        free(entry);
    }
    else {
        int i;
        if (mask & (1 << 1))
            i = 0;
        else
            i = 2;
        for (; i < length; ++i) {
            if (i_nodes[entry[i].i].i_type == DIRTYPE)
                printf(BLU);
            printf("%s\n", entry[i].name);
            if (i_nodes[entry[i].i].i_type == DIRTYPE)
                printf(RESET);
            // if (!(i % 2))
            //     printf("\n");
        }
        // printf("\n");
        free(entry);
    }
    return 0;
}

int fs_mkdir (char * p) {
    path decodePath;
    path workPath;
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("no input\n", stderr);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 1) == -1)
        return -1;
    uint16_t find_p;
    i_index_t find_i;
    if (s_search(workPath.i, decodePath.content[decodePath.p - 1], &find_i, &find_p) != -1) {
        fputs("directory exists\n", stderr);
        return -1;
    }
    if (s_newdir(workPath.i, decodePath.content[decodePath.p - 1], &find_i) == -1) {
        return -1;
    }
    return 0;
}

int fs_rmdir(char * p) {
    path decodePath;
    path workPath;
    char *save_p = (char *)malloc(sizeof(char) * (strlen(p) + 1));
    strcpy(save_p, p);
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("no input\n", stderr);
        free(save_p);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 1) == -1)
        return -1;
    uint16_t find_p;
    i_index_t find_i;
    if (!strcmp(decodePath.content[decodePath.p - 1], "..") ||
        !strcmp(decodePath.content[decodePath.p - 1], ".")) {
            fputs("\".\" and \"..\" may not be removed\n", stderr);
            free(save_p);
            return -1;
    }
    if (s_search(workPath.i, decodePath.content[decodePath.p - 1], &find_i, &find_p) == -1) {
        fputs("No such directory\n", stderr);
        free(save_p);
        return -1;
    }
    if (i_nodes[find_i].i_type != DIRTYPE) {
        fputs("not a directory\n", stderr);
        free(save_p);
        return -1;
    }
    int length;
    d_entry *entry = s_ls(find_i, &length);
    for (int i = 2; i < length; ++i) {
        char *tmp_p = (char *)malloc(sizeof(char) * (strlen(save_p) + strlen(entry[i].name) + 1));
        strcpy(tmp_p, save_p);
        tmp_p[strlen(save_p)] = '/';
        tmp_p[strlen(save_p) + 1] = '\0';
        strcat(tmp_p, entry[i].name);
        if (i_nodes[entry[i].i].i_type == FILETYPE) {
            if (fs_rm(tmp_p) == -1) {
                free(tmp_p);
                free(save_p);
                return -1;
            }
        }
        else if (i_nodes[entry[i].i].i_type == DIRTYPE) {
            if (fs_rmdir(tmp_p) == -1) {
                free(tmp_p);
                free(save_p);
                return -1;
            }
        }
        free(tmp_p);
    }
    if (s_deletedir(workPath.i, find_i, find_p) == -1) {
        free(save_p);
        return -1;
    }
    free(save_p);
    return 0;
}

int fs_touch(char * p) {
    path decodePath;
    path workPath;
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("no input\n", stderr);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 1) == -1)
        return -1;
    uint16_t find_p;
    i_index_t find_i;
    if (s_search(workPath.i, decodePath.content[decodePath.p - 1], &find_i, &find_p) != -1) {
        fputs("file exists\n", stderr);
        return -1;
    }
    if (s_newFile(workPath.i, decodePath.content[decodePath.p - 1], &find_i) == -1) {
        return -1;
    }
    return 0;
}

int fs_rm(char * p) {
    path decodePath;
    path workPath;
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("no input\n", stderr);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 1) == -1)
        return -1;
    uint16_t find_p;
    i_index_t find_i;
    if (s_search(workPath.i, decodePath.content[decodePath.p - 1], &find_i, &find_p) == -1) {
        fputs("No such file\n", stderr);
        return -1;
    }
    if (i_nodes[find_i].i_type != FILETYPE) {
        fputs("not a file\n", stderr);
        return -1;
    }
    if (s_unlinkFile(workPath.i, find_i, find_p) == -1) {
        return -1;
    }
    return 0;
}

int fs_read(char * p, char * buffer) {
    path decodePath;
    path workPath;
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("no input\n", stderr);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 1) == -1)
        return -1;
    uint16_t find_p;
    i_index_t find_i;
    if (s_search(workPath.i, decodePath.content[decodePath.p - 1], &find_i, &find_p) == -1) {
        fputs("No such file\n", stderr);
        return -1;
    }
    s_read(find_i, buffer);
    return 0;
}

int fs_write(char * p, char * buffer, uint16_t pos) {
    path decodePath;
    path workPath;
    if (s_handlepath(&decodePath, p) == -1) {
        fputs("no input\n", stderr);
        return -1;
    }
    if (fs_decode(&decodePath, &workPath, 1) == -1)
        return -1;
    uint16_t find_p;
    i_index_t find_i;
    if (s_search(workPath.i, decodePath.content[decodePath.p - 1], &find_i, &find_p) == -1) {
        fputs("No such file\n", stderr);
        return -1;
    }
    if (s_write(find_i, buffer, pos) == -1) {
        return -1;
    }
    return 0;
}

int fs_save() {
    return writeThrough();
}

int fs_showBash() {
    char *pwd = fs_pwd();
    printf(RED);
    printf("%s@%s:", uid_name[0], host_name);
    printf(MAG);
    printf("%s ", pwd);
    printf(RED);
    printf("$ ");
    printf(RESET);
    free(pwd);
    return 0;
}