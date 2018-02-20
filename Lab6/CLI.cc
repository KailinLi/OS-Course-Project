#include "FSAPI.h"

int main (int argc, char *argv[]) {
    fs_init();
    int l;
    d_entry *list = s_ls(current_i, &l);
    for (int i = 0; i < l; ++i) {
        puts(list[i].name);
    }
    free(list);
}