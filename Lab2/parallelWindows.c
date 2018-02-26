#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main () {
    pid_t pid_0, pid_1, pid_2, pid_3;
    if ((pid_0 = fork()) < 0) {
        fputs("Fork children 0 error\n", stderr);
        exit (0);
    }
    else if (pid_0 == 0) {
        // children 0
        char app[] = "./QShowTime/QShowTime";
        char * const argv[] = { app, "success", NULL };
        if (execv(app, argv) < 0) {
            perror("execv error");
        }
    }
    else {
        if ((pid_1 = fork()) < 0) {
            fputs("Fork children 1 error\n", stderr);
            exit (0);
        }
        else if (pid_1 == 0) {
            // children 1
            char app[] = "./QShowAdd/QShowAdd";
            char * const argv[] = { app, "success", NULL };
            if (execv(app, argv) < 0) {
                perror("execv error");
            }
        }
        else {
            if ((pid_2 = fork()) < 0) {
                fputs("Fork children 2 error\n", stderr);
                exit (0);
            }
            else if (pid_2 == 0) {
                // children 2
                char app[] = "./QShowCPU/QShowCPU";
                char * const argv[] = { app, "success", NULL };
                if (execv(app, argv) < 0) {
                    perror("execv error");
                }
            }
            else {
                if ((pid_3 = fork()) < 0) {
                    fputs("Fork children 3 error\n", stderr);
                    exit (0);
                }
                else if (pid_3 == 0) {
                    // children 3
                    char app[] = "../Lab1/CopyFile/CopyFile";
                    char * const argv[] = { app, "success", NULL };
                    if (execv(app, argv) < 0) {
                        perror("execv error");
                    }
                }
                else {
                    // father
                    if (waitpid(pid_0, NULL, 0) < 0 ||
                        waitpid(pid_1, NULL, 0) < 0 ||
                        waitpid(pid_2, NULL, 0) < 0 ||
                        waitpid(pid_3, NULL, 0) < 0) {
                            fputs("Wait error\n", stderr);
                            exit (0);
                        }
                    fputs("Simulation success\n", stdout);
                    return 0;
                }
            }
        }
    }
}