#ifndef FSBASE_H
#define FSBASE_H

#include "AllocManager.h"

extern char current[4096];
extern const int ENTRYSIZE;
extern i_index_t current_i;

typedef struct D_entry {
    i_index_t i;
    char name[30];
} d_entry;

int s_newFileNode(i_index_t *, b_index_t *);
void s_cleanINode(i_index_t);

int s_addEntry(i_index_t, char *, i_index_t);

d_entry * s_ls(i_index_t, int *);

#endif // FSBASE_H