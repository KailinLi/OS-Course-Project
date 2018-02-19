#ifndef ALLOCMANAGER_H
#define ALLOCMANAGER_H

#include "StoreManager.h"

i_index_t i_alloc();
void i_free(i_index_t);

b_index_t b_alloc();
void b_free(b_index_t);

#endif // ALLOCMANAGER_H