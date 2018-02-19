#include "FSAPI.h"

int fs_init() {
    printf("Making dev...\n");
    if (initSpace() == -1)
        return -1;
    printf("Opening dev...\n");
    if (openFp() == -1)
        return -1;
    printf("Making root...\n");
    i_index_t i;
    b_index_t b;
    if (s_newFileNode(&i, &b) == -1) {
        fputs("root error\n", stderr);
        return -1;
    }
    
}