#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Color.h"

typedef uint16_t block_t;
typedef uint16_t i_index_t;
typedef uint16_t b_index_t;

typedef struct I_node {
    uint16_t i_size;
    uint16_t i_nlink;
    uint8_t i_type;
    uint8_t i_right[3];
    uint8_t i_uid;
    uint8_t i_gid;
    block_t i_block[8];
    uint8_t i_fill[6];
} i_node;

extern const int BLOCKSIZE;
extern const int HEADSIZE;

extern uint8_t i_bitmap[1 << 13];
extern uint8_t b_bitmap[1 << 13];

extern i_node i_nodes[1 << 16];

extern FILE *fp;

int initSpace();
int openFp();
int writeThrough();

#endif // STOREMANAGER_H