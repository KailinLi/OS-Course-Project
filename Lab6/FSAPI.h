#ifndef FSAPI_H
#define FSAPI_H

#include "FSBase.h"

extern char inputName[30];

int fs_init();
char * fs_pwd();

// int fs_touch(i_index_t, char *);

// d_entry * fs_ls();


#endif // FSAPI_H