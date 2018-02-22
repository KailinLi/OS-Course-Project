#include "FSAPI.h"

int main (int argc, char *argv[]) {
    // fs_init();
    // char name[20] = "hello";
    // i_index_t i;
    // s_newFile(current_i, name, &i);
    // strcpy(name, "world");
    // i_index_t j;
    // s_newFile(current_i, name, &j);
    // s_unlinkFile(current_i, i, 2);
    // strcpy(name, "hehe");
    // s_newFile(current_i, name, &i);

    // char name[30] = "Desktop/vim/hehe";
    // path p;
    // printf("%d\n", s_handlepath(&p, name));
    // for (int i = 0; i < p.p; ++i) {
    //     puts(p.content[i]);
    //     // puts("\n");
    // }
    // fs_load();
    // fs_init();
    // fs_init();


    fs_load();
    char input[20] = "big mouse";
    char name[20] = "hello";
    i_index_t i;
    s_newFile(current.i, name, &i);
    strcpy(name, "world");
    i_index_t j;
    s_newFile(current.i, name, &j);
    s_write(j, input, 5);
    strcpy(input, "cat");
    s_write(j, input, 0);
    s_read(j, name);

    
    // puts(name);

    fs_ls("", 3);

    strcpy(input, "./dir");

    fs_mkdir(input);
    fs_ls("", 3);

    strcpy(input, "../world");

    fs_rmdir(input);
    fs_ls("", 3);

    strcpy(input, "../dir");

    fs_cd(input);

    fs_ls("", 3);

    strcpy(input, "../hello");
    fs_rm(input);
    fs_ls("", 3);
    strcpy(input, "..");
    fs_cd(input);
    fs_ls("", 3);


    // s_unlinkFile(current.i, i, 2);
    // strcpy(name, "hehe");
    // s_newdir(current.i, name, &i);
    // int l;
    // d_entry *list = s_ls(0, &l);
    // for (int i = 0; i < l; ++i) {
    //     puts(list[i].name);
    // }
    // free(list);
    // char *tmp  = fs_pwd();
    // puts(tmp);
    // free(tmp);

    // fs_cd(name);

    // // free(list);
    // tmp  = fs_pwd();
    // puts(tmp);
    // free(tmp);

    // strcpy(name, "..");
    // fs_cd(name);
    // tmp  = fs_pwd();
    // puts(tmp);
    // free(tmp);

    // strcpy(name, "world");
    // fs_cd(name);
    // tmp  = fs_pwd();
    // puts(tmp);
    // free(tmp);
    // writeThrough();
}