#include <stdio.h>
#include <stdlib.h>
#define MYCOPYFILE 233
int main (int argc, char *argv[]) {
    if (argc != 3) {
        fputs("argument error\n", stderr);
        exit (1);
    }
    syscall(MYCOPYFILE, argv[1], argv[2]);
    return 0;
}