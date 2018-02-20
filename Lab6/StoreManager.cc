#include "StoreManager.h"

const int INODESIZE = 32;
const int BLOCKSIZE = 1024;
const int HEADSIZE = ((1 << 3) * 2 + (1 << 11));

uint8_t i_bitmap[1 << 13];
uint8_t b_bitmap[1 << 13];

i_node i_nodes[1 << 16];

FILE *fp;

int initSpace() {
    FILE *tmpFp;
    tmpFp = fopen("dev", "w");
    if (tmpFp == NULL) {
        fputs("create dev error\n", stderr);
        return -1;
    }
    uint8_t buffer[BLOCKSIZE];
    // memset(buffer, 0xff, sizeof(buffer));
    // for (int i = 0; i < 16; ++i) {
    //     if (fwrite(buffer, sizeof(uint8_t), BLOCKSIZE, tmpFp) != BLOCKSIZE) {
    //         fputs("write bitmap error\n", stderr);
    //         return -1;
    //     }
    // }
    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < (1 << 16); ++i) {
        if (fwrite(buffer, sizeof(uint8_t), BLOCKSIZE, tmpFp) != BLOCKSIZE) {
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
        if (fread(i_bitmap + i * BLOCKSIZE, sizeof(uint8_t), BLOCKSIZE, fp) != BLOCKSIZE) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
    }
    for (int i = 0; i < 8; ++i) {
        if (fread(b_bitmap + i * BLOCKSIZE, sizeof(uint8_t), BLOCKSIZE, fp) != BLOCKSIZE) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
    }
    uint8_t buffer[BLOCKSIZE];
    for (int i = 0; i < (1 << 11); ++i) {
        if (fread(buffer, sizeof(uint8_t), BLOCKSIZE, fp) != BLOCKSIZE) {
            fputs("read bitmap error\n", stderr);
            return -1;
        }
        for (int j = 0; j < 32; ++j) {
            i_nodes[i * 32 + j].i_size = *(uint16_t*) &buffer[j * INODESIZE];
            i_nodes[i * 32 + j].i_nlink = *(uint16_t*) &buffer[j * INODESIZE + 2];
            i_nodes[i * 32 + j].i_type = buffer[j * INODESIZE + 4];
            for (int k = 0; k < 3; ++k)
                i_nodes[i * 32 + j].i_right[k] = buffer[j * INODESIZE + 5 + k];
            i_nodes[i * 32 + j].i_uid = buffer[j * INODESIZE + 8];
            i_nodes[i * 32 + j].i_gid = buffer[j * INODESIZE + 9];
            for (int k = 0; k < 8; ++k)
                i_nodes[i * 32 + j].i_block[k] = *(block_t*) &buffer[j * INODESIZE + 10 + k * 2];
        }
    }
    return 0;
}


int writeThrough() {
    rewind(fp);
    for (int i = 0; i < 8; ++i) {
        if (fwrite(i_bitmap + i * BLOCKSIZE, sizeof(uint8_t), BLOCKSIZE, fp) != BLOCKSIZE) {
            fputs("write bitmap error\n", stderr);
            return -1;
        }
    }
    for (int i = 0; i < 8; ++i) {
        if (fwrite(b_bitmap + i * BLOCKSIZE, sizeof(uint8_t), BLOCKSIZE, fp) != BLOCKSIZE) {
            fputs("write bitmap error\n", stderr);
            return -1;
        }
    }
    uint8_t buffer[BLOCKSIZE];
    for (int i = 0; i < (1 << 11); ++i) {
        memset(buffer, 0, sizeof(buffer));
        for (int j = 0; j < 32; ++j) {
            *(uint16_t*) &buffer[j * INODESIZE] = i_nodes[i * 32 + j].i_size;
            *(uint16_t*) &buffer[j * INODESIZE + 2] = i_nodes[i * 32 + j].i_nlink;
            buffer[j * INODESIZE + 4] = i_nodes[i * 32 + j].i_type;
            for (int k = 0; k < 3; ++k)
                buffer[j * INODESIZE + 5 + k] = i_nodes[i * 32 + j].i_right[k];
            buffer[j * INODESIZE + 8] = i_nodes[i * 32 + j].i_uid;
            buffer[j * INODESIZE + 9] = i_nodes[i * 32 + j].i_gid;
            for (int k = 0; k < 8; ++k)
                *(block_t*) &buffer[j * INODESIZE + 10 + k * 2] = i_nodes[i * 32 + j].i_block[k];
        }
        if (fwrite(buffer, sizeof(uint8_t), BLOCKSIZE, fp) != BLOCKSIZE) {
            fputs("write bitmap error\n", stderr);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}