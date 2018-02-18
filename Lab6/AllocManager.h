#ifndef ALLOCMANAGER_H
#define ALLOCMANAGER_H

#include "StoreManager.h"

i_node * i_alloc();
int i_free(i_node *);

#endif // ALLOCMANAGER_H