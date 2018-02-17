#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#define MYCOPYFILE 333
int main (int argc, char *argv[]) {
    if (argc != 3) {
        fputs("argument error\n", stderr);
        exit (1);
    }
    syscall(MYCOPYFILE, argv[1], argv[2]);
    return 0;
}
