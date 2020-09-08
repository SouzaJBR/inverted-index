//
// Created by souzajbr on 07/09/2020.
//

#ifndef TRABALHO_AED_HASH_H
#define TRABALHO_AED_HASH_H

#include <stdlib.h>
#include "document.h"

typedef long (*hash_function)(char*);

struct hash_table_list {
    struct hash_table_list* next;
    struct document* doc;
};

struct hash_table {
    struct hash_table_list** table;
    int length;
};

struct hash_table* hash_table_create(int length);
void hash_table_insert(struct hash_table* hashTable, char* key, struct document* doc, hash_function hash);

#endif //TRABALHO_AED_HASH_H
