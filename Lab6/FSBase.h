#ifndef FSBASE_H
#define FSBASE_H

#include "AllocManager.h"

extern char current[4096];

int s_newFileNode(i_index_t *, b_index_t *);
void s_cleanINode(i_index_t);

int s_addEntry(i_index_t, char *, i_index_t);

#endif // FSBASE_H