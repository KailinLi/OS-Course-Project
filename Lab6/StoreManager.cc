#include "StoreManager.h"

int initSpace() {
    FILE *fp;
    fp = fopen("dev", "w");
    if (fp == NULL) {
        fputs("create dev error\n", stderr);
        return -1;
    }
    uint8_t buffer[1024];
    for (int i = 0; i < (1 << 10); ++i) {
        if (fwrite(buffer, sizeof(uint8_t), sizeof(buffer), fp) != 1024) {
            fputs("write error\n", stderr);
            return -1;
        }
    }
    return 0;
}