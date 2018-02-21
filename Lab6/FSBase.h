#ifndef FSBASE_H
#define FSBASE_H

#include "AllocManager.h"

typedef struct Path {
    char content[140][30];
    uint16_t p;
    i_index_t i;
} path;

// extern char current[140][30];
// extern uint16_t current_p;
extern path current;

extern const int ENTRYSIZE;
// extern i_index_t current_i;

typedef struct D_entry {
    i_index_t i;
    char name[30];
} d_entry;

int s_newNode(i_index_t *, b_index_t *);
void s_cleanINode(i_index_t);

int s_addEntry(i_index_t, char *, i_index_t);

d_entry * s_ls(i_index_t, int *);

void s_delete(i_index_t, i_index_t, uint16_t);

int s_newFile(i_index_t, char *, i_index_t *);
int s_unlinkFile(i_index_t, i_index_t, uint16_t);

int s_newdir(i_index_t, char *, i_index_t *);
int s_deletedir(i_index_t, i_index_t, uint16_t);

int s_changedir(path *, char *, i_index_t);

int s_handlepath(path *, char *);

#endif // FSBASE_H