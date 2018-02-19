#include "FSBase.h"

char current[4096];

int s_newFileNode(i_index_t *i, b_index_t *b) {
    if ((*i = i_alloc()) == 0xffff)
        return -1;
    if ((*b = b_alloc()) == 0xffff) {
        i_free(*i);
        return -1;
    }
    s_cleanINode(*i);
    i_nodes[*i].i_block[0] = *b;
    return 0;
}
void s_cleanINode(i_index_t i) {
    i_nodes[i].i_size = 0;
    i_nodes[i].i_nlink = 0;
    i_nodes[i].i_type = 0;
    memset(i_nodes[i].i_right, 0, sizeof(i_nodes[i].i_right));
    i_nodes[i].i_uid = 0;
    i_nodes[i].i_gid = 0;
    memset(i_nodes[i].i_block, 0, sizeof(i_nodes[i].i_block));
}

int s_addEntry(i_index_t d, char *name, i_index_t i) {
    if (i_nodes[d].i_size == (1 << 13)) {
        fputs("directory is full\n", stderr);
        return -1;
    }
    b_index_t b;
    if (!(i_nodes[d].i_size % BLOCKSIZE) && !i_nodes[d].i_size)
        b = b_alloc();
    else
        b = i_nodes[d].i_block[i_nodes[d].i_size / BLOCKSIZE];
    int offset = i_nodes[d].i_size % BLOCKSIZE;
    fseek(fp, b * BLOCKSIZE + offset + HEADSIZE, SEEK_SET);
    

}