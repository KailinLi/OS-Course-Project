#include "StoreManager.h"

uint8_t i_bitmap[1 << 13];
uint8_t b_bitmap[1 << 13];

FILE *fp;

int initSpace() {
    FILE *tmpFp;
    tmpFp = fopen("dev", "w");
    if (tmpFp == NULL) {
        fputs("create dev error\n", stderr);
        return -1;
    }
    uint8_t buffer[1024];
    for (int i = 0; i < (1 << 16); ++i) {
        if (fwrite(buffer, sizeof(uint8_t), sizeof(buffer), tmpFp) != 1024) {
            fputs("write error\n", stderr);
            return -1;
        }
    }
    return 0;
}

int openFp() {
    fp = fopen("dev", "r+");
    if (fp == NULL) {
        fputs("open dev error\n", stderr);
        return -1;
    }
    for (int i = 0; i < 8; ++i) {
        if (fread(i_bitmap + i * 1024, sizeof(uint8_t), 1024, fp) != 1024) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
    }
    for (int i = 0; i < 8; ++i) {
        if (fread(b_bitmap + i * 1024, sizeof(uint8_t), 1024, fp) != 1024) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
    }
    return 0;
}


int writeThrough() {
    rewind(fp);
    for (int i = 0; i < 8; ++i) {
        if (fwrite(i_bitmap + i * 1024, sizeof(uint8_t), 1024, fp) != 1024) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
    }
    for (int i = 0; i < 8; ++i) {
        if (fwrite(b_bitmap + i * 1024, sizeof(uint8_t), 1024, fp) != 1024) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
    }
    return 0;
}