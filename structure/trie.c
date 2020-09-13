//
// Created by souzajbr on 07/09/2020.
//


#include "trie.h"

struct trie* trie_create() {

    struct trie* new = malloc(sizeof(struct trie));
    for(int i = 0; i < TRIE_ALPHABET_SIZE; i++)
        new->root.child[i] = NULL;

    return new;

}

char trie_calculate_position(char p) {

    if(p >= '0' && p <= '9')
        return p - 48;

    return (unsigned int) (p - 97) + 10;
}

struct trie_node* trie_insert_aux(struct trie_node* root, char* key, int pos, int length, struct document* doc) {

    if (root == NULL) {
        root = malloc(sizeof(struct trie_node));
        for(int i = 0; i < TRIE_ALPHABET_SIZE; i++)
            root->child[i] = NULL;
        root->isKey = false;
    }

    if(pos + 1 == length) {
        root->isKey = true;
        root->docs = document_list_insert_aux(root->docs, doc);
    }

    if(pos + 1 < length) {
        int child =  ((unsigned int) trie_calculate_position(key[pos]) % TRIE_ALPHABET_SIZE);
        root->child[child] = trie_insert_aux(root->child[child], key, pos + 1, length, doc);
    }
    return root;
}

void trie_insert(struct trie* trie, char* key, struct document* doc) {

    int length = strlen(key);
    trie_insert_aux(&trie->root, key, 0, length, doc);
}

struct document_list* trie_search_aux(struct trie_node* root, char* keyword, int pos, int length) {
    if (root == NULL) return NULL;

    if(pos + 1 == length)
        return root->docs;

    int child =  ((unsigned int) trie_calculate_position(keyword[pos]) % TRIE_ALPHABET_SIZE);
    return trie_search_aux(root->child[child], keyword, pos + 1, length);
}

struct document_list* trie_search(struct trie* trie, char* keyword) {

    if(trie != NULL && keyword != NULL && strcmp("", keyword) != 0)
        return trie_search_aux(&trie->root, keyword, 0, strlen(keyword));

    return NULL;
}