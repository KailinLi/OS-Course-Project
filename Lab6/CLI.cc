#include "FSAPI.h"

int main (int argc, char *argv[]) {
    // printf("%ld\n", sizeof(i_node));
    // i_bitmap[0] = 9;
    // printf("%d\n", i_bitmap[0]);
    openFp();
    i_bitmap[0] = 0xff;
    i_bitmap[1] = 0xf7;
    printf("%d\n", i_alloc());
    printf("%x\n", i_bitmap[1]);
    i_free(7);
    i_free(5);
    i_free(14);
    printf("%x %x\n", i_bitmap[0], i_bitmap[1]);
    // printf("%d %d\n", i_bitmap[0], b_bitmap[1]);
    // i_bitmap[0] = 9;
    // b_bitmap[1] = 10;
    // printf("%d\n", i_bitmap[10]);
    // printf("%x %x %x %x %x %x\n", i_nodes[1 << 15].i_block[7], i_nodes[1 << 15].i_size, 
    //     i_nodes[1 << 15].i_type, i_nodes[1 << 15].i_right[1], i_nodes[1 << 15].i_gid, 
    //     i_nodes[1 << 15].i_block[3]);
    // i_nodes[1 << 15].i_block[7] = 0xabff;
    // i_nodes[1 << 15].i_size = 0x9ABC;
    // i_nodes[1 << 15].i_type = 0x89;
    // i_nodes[1 << 15].i_right[1] = 0x78;
    // i_nodes[1 << 15].i_gid = 0x67;
    // i_nodes[1 << 15].i_block[3] = 0xab67;
    // for (int i = 0; i < 8; ++i) {
    //     printf("%x\n", i_nodes[1 << 14].i_block[i]);
    // }
    // for (int i = 0; i < 8; ++i) {
    //     i_nodes[1 << 14].i_block[i] = 0xabcd;
    // }
    // writeThrough();
    // initSpace();
    // uint8_t i = 12;
    // printf("%d\n", (i + 1) & ~i);
    // FILE *fp;
    // unsigned int size;
    // char *buffer;
    // fp = fopen("dev", "r");
    // if (fp == NULL) {
    //     fputs("Read file error\n", stderr);
    //     exit (1);
    // }
    // fseek(fp, 0, SEEK_END);
    // size = (unsigned int)ftell(fp);
    // printf("%d", size);
}