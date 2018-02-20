#include "FSBase.h"

char current[4096];
i_index_t current_i;
const int ENTRYSIZE = (1 << 5);

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
    if (!(i_nodes[d].i_size % BLOCKSIZE) && !i_nodes[d].i_size) {
        b = b_alloc();
        i_nodes[d].i_block[i_nodes[d].i_size / BLOCKSIZE] = b;
    }
    else
        b = i_nodes[d].i_block[i_nodes[d].i_size / BLOCKSIZE];
    int offset = i_nodes[d].i_size % BLOCKSIZE;
    fseek(fp, b * BLOCKSIZE + offset + HEADSIZE * BLOCKSIZE, SEEK_SET);
    uint8_t buffer[2];
    *(i_index_t*)&buffer[0] = i;
    fwrite(buffer, sizeof(uint8_t), 2, fp);
    fwrite(name, sizeof(uint8_t), strlen(name), fp);
    i_nodes[d].i_size += ENTRYSIZE;
    
    // add link
    ++i_nodes[i].i_nlink;
    return 0;
}

d_entry * s_ls(i_index_t i, int *length) {
    int size = i_nodes[i].i_size / ENTRYSIZE;
    d_entry *list = (d_entry *)malloc(sizeof(d_entry) * size);
    block_t current_b = i_nodes[i].i_block[0];
    fseek(fp, current_b * BLOCKSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
    uint8_t buffer[32];
    for (int k = 0; k < size; ++k) {
        fread(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
        list[k].i = *(i_index_t*) &buffer[0];
        strcpy(list[k].name, (char *)& buffer[2]);
        if (!(k + 1) % (BLOCKSIZE / ENTRYSIZE))
            current_b = i_nodes[i].i_block[(k + 1) / (BLOCKSIZE / ENTRYSIZE)];
    }
    *length = size;
    return list;
}