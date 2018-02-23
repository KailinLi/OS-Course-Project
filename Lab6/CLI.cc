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
        if(!strcmp(param[0], "exit")) {
            fs_save();
            break;
        }
        else if (!strcmp(param[0], "q")) {
            fclose(fp);
            break;
        }
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
                if (fs_read(input, buffer) != -1)
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
        else if (!strcmp(param[0], "vim")) {
            if (length == 1) {
                fputs("usage: vim file ...\n", stderr);
                continue;
            }
            pid_t pid;
            strcpy(input, param[1]);
            char *buffer = (char *)malloc(sizeof(char) * (1 << 13));
            fs_read(input, buffer);
            FILE *tmpfp = fopen("tmpfile", "w");
            if (tmpfp == NULL) {
                fputs("open file error\n", stderr);
                free(buffer);
                continue;
            }
            if (fwrite(buffer, sizeof(char), strlen(buffer) + 1, tmpfp) != strlen(buffer) + 1) {
                fputs("copy file error\n", stderr);
                free(buffer);
                continue;
            }
            fclose(tmpfp);
            free(buffer);
            if ((pid = fork()) < 0) {
                fputs("fork error\n", stderr);
                continue;
            }
            else if (pid == 0) {
                char app[] = "/usr/bin/vim";
                strcpy(param[1], "/usr/bin/vim");
                strcpy(param[2], "./tmpfile");
                char * const argv[] = {param[1], param[2], NULL};
                if (execv(app, argv) < 0) {
                    perror("execv error");
                }
            }
            else {
                if (waitpid(pid, NULL, 0) < 0) {
                    fputs("waitpid error\n", stderr);
                    continue;
                }
                tmpfp = fopen("tmpfile", "r");
                if (tmpfp == NULL) {
                    fputs("open file error\n", stderr);
                    continue;
                }
                fseek(tmpfp, 0, SEEK_END);
                unsigned int size = ftell(tmpfp);
                rewind(tmpfp);
                buffer = (char *)malloc(sizeof(char) * size);
                if (buffer == NULL) {
                    fputs("Malloc error\n", stderr);
                    continue;
                }
                if (size != fread(buffer, sizeof(char), size, tmpfp)) {
                    fputs("Read error\n", stderr);
                    continue;
                }
                fclose(tmpfp);
                if (remove("./tmpfile") != 0) {
                    fputs("can not delete tmpfile\n", stderr);
                    free(buffer);
                    continue;
                }
                fs_write(input, buffer, 0);
                free(buffer);
            }
        }
        else if (!strcmp(param[0], "chmod")) {
            if (length != 3) {
                fputs("usage: chmod [0-7][0-7][0-7] file ...\n", stderr);
                continue;
            }
            fs_chmod(param[2], param[1]);
        }
        else if (!strcmp(param[0], "adduser")) {
            if (length != 2) {
                fputs("usage: adduser new username ...\n", stderr);
                continue;
            }
            fs_adduser(param[1]);
        }
        else if (!strcmp(param[0], "userdel")) {
            if (length != 2) {
                fputs("usage: userdel username ...\n", stderr);
                continue;
            }
            fs_deleteuser(param[1]);
        }
        else if (!strcmp(param[0], "su")) {
            if (length == 1) {
                strcpy(param[1], "root");
            }
            else if (length != 2) {
                fputs("usage: su [username] ...\n", stderr);
                continue;
            }
            fs_changeuser(param[1]);
        }
        else
            printf("command not found\n");
    }
    free(input);
}