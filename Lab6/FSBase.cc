#include "FSBase.h"

// char current[140][30];
// uint16_t current_p = 0;
path current;
// i_index_t current_i;

const uint8_t DIRTYPE = 0;
const uint8_t FILETYPE = 1;

const uint8_t uid = 0;
const uint8_t gid = 0;

uint8_t default_file_right[3] = {6, 6, 4};
uint8_t default_dir_right[3] = {7, 6, 4};

const int ENTRYSIZE = (1 << 5);

int s_newNode(i_index_t *i, b_index_t *b) {
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
    uint16_t offset = i_nodes[d].i_size % BLOCKSIZE;
    fseek(fp, b * BLOCKSIZE + offset + HEADSIZE * BLOCKSIZE, SEEK_SET);
    uint8_t buffer[2];
    *(i_index_t*)&buffer[0] = i;
    fwrite(buffer, sizeof(uint8_t), 2, fp);
    fwrite(name, sizeof(uint8_t), strlen(name) + 1, fp);
    i_nodes[d].i_size += ENTRYSIZE;
    
    // add link
    ++i_nodes[i].i_nlink;
    return 0;
}

d_entry * s_ls(i_index_t d, int *length) {
    uint16_t size = i_nodes[d].i_size / ENTRYSIZE;
    d_entry *list = (d_entry *)malloc(sizeof(d_entry) * size);
    block_t current_b = i_nodes[d].i_block[0];
    fseek(fp, current_b * BLOCKSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
    uint8_t buffer[ENTRYSIZE];
    for (int i = 0; i < size; ++i) {
        fread(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
        list[i].i = *(i_index_t*) &buffer[0];
        strcpy(list[i].name, (char *)& buffer[2]);
        if (!(i + 1) % (BLOCKSIZE / ENTRYSIZE))
            current_b = i_nodes[d].i_block[(i + 1) / (BLOCKSIZE / ENTRYSIZE)];
    }
    *length = size;
    return list;
}

void s_delete(i_index_t d, i_index_t i, uint16_t pos) {
    for (int k = 0; k <= i_nodes[i].i_size / BLOCKSIZE; ++k) {
        b_free(i_nodes[i].i_block[k]);
    }
    uint16_t size = i_nodes[d].i_size / BLOCKSIZE;
    uint16_t offset = i_nodes[d].i_size - (BLOCKSIZE * size) - ENTRYSIZE;
    block_t b = i_nodes[d].i_block[size];
    fseek(fp, b * BLOCKSIZE + offset + HEADSIZE * BLOCKSIZE, SEEK_SET);
    uint8_t buffer[ENTRYSIZE];
    fread(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
    b = i_nodes[d].i_block[pos / (BLOCKSIZE / ENTRYSIZE)];
    offset = pos % (BLOCKSIZE / ENTRYSIZE);
    fseek(fp, b * BLOCKSIZE + offset * ENTRYSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
    fwrite(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
    i_nodes[d].i_size -= ENTRYSIZE;
    i_free(i);
}

int s_newFile(i_index_t d, char *name, i_index_t *new_i) {
    b_index_t new_b;
    if (s_newNode(new_i, &new_b) == -1) {
        fputs("can not new file\n", stderr);
        return -1;
    }
    if (s_addEntry(d, name, *new_i) == -1) {
        i_free(*new_i);
        b_free(new_b);
        fputs("can not new file\n", stderr);
        return -1;
    }
    i_nodes[*new_i].i_type = FILETYPE;
    i_nodes[*new_i].i_right[0] = default_file_right[0];
    i_nodes[*new_i].i_right[1] = default_file_right[1];
    i_nodes[*new_i].i_right[2] = default_file_right[2];
    return 0;
}

int s_unlinkFile(i_index_t d, i_index_t i, uint16_t pos) {
    if (--i_nodes[i].i_nlink == 0) {
        s_delete(d, i, pos);
        // for (int k = 0; k <= i_nodes[i].i_size / BLOCKSIZE; ++k) {
        //     b_free(i_nodes[i].i_block[k]);
        // }
        // uint16_t size = i_nodes[d].i_size / BLOCKSIZE;
        // uint16_t offset = i_nodes[d].i_size - (BLOCKSIZE * size) - ENTRYSIZE;
        // block_t b = i_nodes[d].i_block[size];
        // fseek(fp, b * BLOCKSIZE + offset + HEADSIZE * BLOCKSIZE, SEEK_SET);
        // uint8_t buffer[ENTRYSIZE];
        // fread(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
        // b = i_nodes[d].i_block[pos / (BLOCKSIZE / ENTRYSIZE)];
        // offset = pos % (BLOCKSIZE / ENTRYSIZE);
        // fseek(fp, b * BLOCKSIZE + offset * ENTRYSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
        // fwrite(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
        // i_nodes[d].i_size -= ENTRYSIZE;
        // i_free(i);

        // uint16_t size = i_nodes[d].i_size / ENTRYSIZE;
        // block_t current_b = i_nodes[d].i_block[0];
        // fseek(fp, current_b * BLOCKSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
        // uint8_t buffer[32];
        // int k;
        // for (k = 0; k < size; ++k) {
        //     fread(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
        //     if (k == *(i_index_t*) &buffer[0])
        //         break;
        //     if (!(k + 1) % (BLOCKSIZE / ENTRYSIZE))
        //         current_b = i_nodes[d].i_block[(k + 1) / (BLOCKSIZE / ENTRYSIZE)];
        // }
        // size
    }
    return 0;
}

int s_newdir(i_index_t d, char * name, i_index_t *new_i) {
    b_index_t new_b;
    if (s_newNode(new_i, &new_b) == -1) {
        fputs("can not new dir\n", stderr);
        return -1;
    }
    if (s_addEntry(d, name, *new_i) == -1) {
        i_free(*new_i);
        b_free(new_b);
        fputs("can not new dir\n", stderr);
        return -1;
    }
    char base_name[30];
    strcpy(base_name, ".");
    if (s_addEntry(*new_i, base_name, *new_i) == -1) {
        i_free(*new_i);
        b_free(new_b);
        fputs("can not new dir\n", stderr);
        return -1;
    }
    strcpy(base_name, "..");
    if (s_addEntry(*new_i, base_name, d) == -1) {
        i_free(*new_i);
        b_free(new_b);
        fputs("can not new dir\n", stderr);
        return -1;
    }
    i_nodes[*new_i].i_type = DIRTYPE;
    i_nodes[*new_i].i_right[0] = default_dir_right[0];
    i_nodes[*new_i].i_right[1] = default_dir_right[1];
    i_nodes[*new_i].i_right[2] = default_dir_right[2];
    return 0;
}

int s_deletedir(i_index_t d, i_index_t i, uint16_t pos) {
    --i_nodes[d].i_nlink;
    s_delete(d, i, pos);
    return 0;
}

int s_changedir(path * p, char * name, i_index_t i) {
    if (!strcmp(name, ".")) {
        return 0;
    }
    else if (!strcmp(name, "..")) {
        if (p->p == 1)
            return 0;
        --p->p;
        p->i = i;
    }
    else {
        strcpy(p->content[p->p++], name);
        p->i = i;
        return 0;
    }
    return 0;
}

int s_handlepath(path * p, char *input) {
    p->p = 0;
    p->i = 0;
    if (strlen(input) == 0) return -1;
    int pos = 0;
    int before_pos = 0;
    int i = 0;
    if (input[0] == '/') {
        strcpy(p->content[i++], "/");
        ++pos;
        ++before_pos;
    }
    int size = strlen(input) + 1;
    while (pos < size) {
        while (input[pos] != '/' && input[pos] != '\0')
            ++pos;
        input[pos] = '\0';
        if (pos == before_pos) return -1;
        strcpy(p->content[i++], input + before_pos);
        before_pos = ++pos;
    }
    p->p = i;
    return 0;
}

int s_search(i_index_t d, char * name, i_index_t *i, uint16_t *pos) {
    uint16_t size = i_nodes[d].i_size / ENTRYSIZE;
    block_t current_b = i_nodes[d].i_block[0];
    fseek(fp, current_b * BLOCKSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
    uint8_t buffer[32];
    int k;
    for (k = 0; k < size; ++k) {
        fread(buffer, sizeof(uint8_t), ENTRYSIZE, fp);
        if (!strcmp(((char*) &buffer[2]), name)) {
            *i = *(i_index_t *)&buffer[0];
            *pos = k;
            return 0;
        }
        if (!(k + 1) % (BLOCKSIZE / ENTRYSIZE))
            current_b = i_nodes[d].i_block[(k + 1) / (BLOCKSIZE / ENTRYSIZE)];
    }
    return -1;
}

int s_write(i_index_t i, char * input, uint16_t pos) {
    if (pos < i_nodes[i].i_size)
        i_nodes[i].i_size = pos;
    if (i_nodes[i].i_size + strlen(input) + 1 > (1 << 13)) {
        fputs("file is full\n", stderr);
        return -1;
    }
    b_index_t b = i_nodes[i].i_block[i_nodes[i].i_size / BLOCKSIZE];
    uint16_t leftsize = strlen(input) + 1;
    uint16_t offset = i_nodes[i].i_size % BLOCKSIZE;
    uint16_t leftSpace = BLOCKSIZE - offset;
    uint16_t write_offset = 0;
    while (leftsize > 0) {
        if (leftsize <= leftSpace) {
            fseek(fp, b * BLOCKSIZE + offset + HEADSIZE * BLOCKSIZE, SEEK_SET);
            fwrite(input + write_offset, sizeof(char), leftsize, fp);
            write_offset += leftsize;
            i_nodes[i].i_size += leftsize;
            leftsize = 0;
        }
        else {
            fseek(fp, b * BLOCKSIZE + offset + HEADSIZE * BLOCKSIZE, SEEK_SET);
            fwrite(input + write_offset, sizeof(char), leftSpace, fp);
            leftsize -= leftSpace;
            write_offset += leftSpace;
            i_nodes[i].i_size += leftSpace;
            b = b_alloc();
            i_nodes[i].i_block[i_nodes[i].i_size / BLOCKSIZE] = b;
        }
    }
    --i_nodes[i].i_size;    // '\0'
    return 0;
}
int s_read(i_index_t i, char * output) {
    b_index_t b;
    uint16_t read_offset = 0;
    int k = 0;
    for (; k < i_nodes[i].i_size / BLOCKSIZE; ++k) {
        b = i_nodes[i].i_block[k];
        fseek(fp, b * BLOCKSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
        fread(output + read_offset, sizeof(char), BLOCKSIZE, fp);
        read_offset += BLOCKSIZE;
    }
    b = i_nodes[i].i_block[k];
    fseek(fp, b * BLOCKSIZE + HEADSIZE * BLOCKSIZE, SEEK_SET);
    fread(output + read_offset, sizeof(char), i_nodes[i].i_size % BLOCKSIZE + 1, fp);
    return 0;
}