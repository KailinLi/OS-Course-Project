#ifndef FSAPI_H
#define FSAPI_H

#include "FSBase.h"

extern const i_index_t HOMEINODE;
// extern const uint8_t USERSIZE;
#define USERSIZE 10

extern char uid_name[USERSIZE][30];
extern char gid_name[USERSIZE][30];
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

int fs_chmod(char *, char *);

int fs_adduser(char *);
int fs_deleteuser(char *);
int fs_changeuser(char *);

int fs_ln(char *, char *);

int right(i_index_t, uint8_t);

int readUID();
int writeUID();

#endif // FSAPI_H