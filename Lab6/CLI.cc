#include "FSAPI.h"

int main (int argc, char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], "init")) {
        if (fs_init() == -1)
            exit (-1);
        if (fs_save() == -1)
            exit (-1);
        return 0;
    }
    if (fs_load() == -1) {
        exit (-1);
    }
    size_t length = 1000;
    char *input = (char *)malloc(sizeof(char) * length);
    char param [10][1000];
    while (1) {
        fs_showBash();
        getline(&input, &length, stdin);
        if (fs_divide(input, param, &length) == -1)
            continue;
        if(!strcmp(param[0], "exit") || !strcmp(param[0], "q"))
            break;
        else if (!strcmp(param[0], "ls")) {
            uint8_t mask = 0;
            strcpy(input, "");
            if (length == 1)
                strcpy(input, ".");
            else
                for (int i = 1; i < length; ++i) {
                    if (param[i][0] == '-')
                        for (int j = 1; j < strlen(param[i]); ++j) {
                            if (param[i][j] == 'l')
                                mask |= 1;
                            else if (param[i][j] == 'a')
                                mask |= (1 << 1);
                        }
                    else
                        strcpy(input, param[i]);
                }
            fs_ls(input, mask);
        }
        else if (!strcmp(param[0], "cd")) {
            if (length == 1) {
                strcpy(input, "/");
            }
            strcpy(input, param[1]);
            fs_cd(input);
        }
        else if (!strcmp(param[0], "mkdir")) {
            if (length == 1) {
                fputs("usage: mkdir directory ...\n", stderr);
                continue;
            }
            for (int i = 1; i < length; ++i) {
                strcpy(input, param[i]);
                fs_mkdir(input);
            }
        }
        else if (!strcmp(param[0], "rmdir")) {
            if (length == 1) {
                fputs("usage: rmdir directory ...\n", stderr);
                continue;
            }
            for (int i = 1; i < length; ++i) {
                strcpy(input, param[i]);
                fs_rmdir(input);
            }
        }
        else if (!strcmp(param[0], "touch")) {
            if (length == 1) {
                fputs("usage: touch new file ...\n", stderr);
                continue;
            }
            for (int i = 1; i < length; ++i) {
                strcpy(input, param[i]);
                fs_touch(input);
            }
        }
        else if (!strcmp(param[0], "rm")) {
            if (length == 1) {
                fputs("usage: rm file ...\n", stderr);
                continue;
            }
            for (int i = 1; i < length; ++i) {
                strcpy(input, param[i]);
                fs_rm(input);
            }
        }
        else if (!strcmp(param[0], "cat")) {
            if (length == 1) {
                fputs("usage: cat file ...\n", stderr);
                continue;
            }
            for (int i = 1; i < length; ++i) {
                strcpy(input, param[i]);
                char *buffer = (char *)malloc(sizeof(char) * (1 << 13));
                fs_read(input, buffer);
                puts(buffer);
                free(buffer);
            }
        }
        else if (!strcmp(param[0], "echo")) {
            if (length != 4) {
                fputs("usage: echo \"string\" [> | >>] file ...\n", stderr);
                continue;
            }
            strcpy(input, param[1]);
            if (!strcmp(param[2], ">")) {
                fs_write(param[3], input, 0);
            }
            else if (!strcmp(param[2], ">>")) {
                fs_write(param[3], input, 0xffff);
            }
            else {
                fputs("usage: echo \"string\" [> | >>] file ...\n", stderr);
                continue;
            }  
        }
        else
            printf("command not found\n");
    }
    free(input);
}