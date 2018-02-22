#ifndef FSAPI_H
#define FSAPI_H

#include "FSBase.h"

extern char inputName[30];

extern const i_index_t HOMEINODE;

extern char uid_name[20][30];
extern char gid_name[20][30];
extern char host_name[30];

int fs_init();
int fs_load();
char * fs_pwd();

int fs_decode(path *, path *, int);
int fs_divide(char *, char (*)[1000], size_t *);

int fs_cd(char *);
int fs_ls(char *, uint8_t);

int fs_mkdir(char *);
int fs_rmdir(char *);

int fs_touch(char *);
int fs_rm(char *);

int fs_read(char *, char *);
int fs_write(char *, char *, uint16_t);

int fs_save();
int fs_showBash();

// int fs_touch(i_index_t, char *);

// d_entry * fs_ls();


#endif // FSAPI_H