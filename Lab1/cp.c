#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {
    if (argc < 1 || argc > 3) {
        fputs("Check the argument\n", stderr);
    }
    FILE *fp;
    unsigned int size;
    char *buffer;
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fputs("Read file error\n", stderr);
        exit (1);
    }
    fseek(fp, 0, SEEK_END);
    size = (unsigned int)ftell(fp);
    rewind(fp);
    buffer = (char *)malloc(sizeof(char) * size);
    if (buffer == NULL) {
        fputs("Malloc error\n", stderr);
        exit (1);
    }
    if (size != fread(buffer, sizeof(char), size, fp)) {
        fputs("Read error\n", stderr);
        exit (1);
    }
    fclose(fp);
    fp = fopen(argv[2], "wb");
    if (fp == NULL) {
        fputs("File error\n", stderr);
        exit (1);
    }
    if (size != fwrite(buffer, sizeof(char), size, fp)) {
        fputs("Write error\n", stderr);
        exit (1);
    }
    fclose(fp);
    free(buffer);
    return 0;
}