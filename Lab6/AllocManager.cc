#include "AllocManager.h"

i_index_t i_alloc() {
    for (i_index_t i = 0; i < (1 << 13); ++i) {
        uint8_t t = i_bitmap[i];
        for (i_index_t j = 0; j < (1 << 3); ++j) {
            if (!(t & (1 << 7))) {
                i_bitmap[i] |= (1 << (7 - j));
                return i * 8 + j;
            }
            t <<= 1;
        }
    }
    return 0xffff;
}

void i_free(i_index_t index) {
    i_index_t i = index / 8;
    i_index_t j = index - (8 * i);
    i_bitmap[i] &= ~(1 << (7 - j));
}

b_index_t b_alloc() {
    for (b_index_t i = 0; i < (1 << 13); ++i) {
        uint8_t t = b_bitmap[i];
        for (b_index_t j = 0; j < (1 << 3); ++j) {
            if (!(t & (1 << 7))) {
                b_bitmap[i] |= (1 << (7 - j));
                return i * 8 + j;
            }
            t <<= 1;
        }
    }
    return 0xffff;
}

void b_free(b_index_t index) {
    b_index_t i = index / 8;
    b_index_t j = index - (8 * i);
    b_bitmap[i] &= ~(1 << (7 - j));
}