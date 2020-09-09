//
// Created by souzajbr on 07/09/2020.
//

#ifndef TRABALHO_AED_HASH_H
#define TRABALHO_AED_HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "document.h"

#define HASH_TABLE_SIZE 16963

#define HASH_FUNCTION_SIMPLE 1
#define HASH_FUNCTION_POLYNOMIAL_ROLLING 2

typedef unsigned long (*hash_function)(char*);

struct hash_table_list {
    struct hash_table_list* next;
    char* key;
    struct document_list* docs;
};

struct hash_table {
    struct hash_table_list** table;
    int length;
    hash_function hash;
};

struct hash_table* hash_table_create(int length, int hashFunction);
void hash_table_insert(struct hash_table* hashTable, char* key, struct document* doc);

#endif //TRABALHO_AED_HASH_H
