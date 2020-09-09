//
// Created by souzajbr on 07/09/2020.
//

#ifndef TRABALHO_AED_TRIE_H
#define TRABALHO_AED_TRIE_H

#include <stdbool.h>
#include <stdlib.h>

#include "document.h"


struct trie_node {
    bool isKey;
    struct trie_node* child[36]; //0-9 A-Z
    struct document_list* docs;
};

struct trie {

    struct trie_node root;

};

struct trie* trie_create();
void trie_insert(struct trie* trie, char* key, struct document* doc);
char trie_calculate_position(char p);

#endif //TRABALHO_AED_TRIE_H
