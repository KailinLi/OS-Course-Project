#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct I_node {
    uint16_t i_size;
    uint16_t i_nlink;
    uint8_t i_type;
    uint8_t i_right[3];
    uint8_t i_uid;
    uint8_t i_gid;
    uint16_t i_block[8];
    uint8_t i_fill[6];
} i_node;

int initSpace();

#endif // STOREMANAGER_H