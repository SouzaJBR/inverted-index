//
// Created by souzajbr on 07/09/2020.
//

#include <string.h>
#include "hash.h"

/**
 * Polynomial Rolling Hash Function
 * p-> It is reasonable to make p a prime number roughly equal to the number of characters in the input alphabet
 * m-> m should be a large number, since the probability of two random strings colliding is about 1/m
 * Precomputing the powers of p might give a performance boost.
 * @param key is the key to be hashed
 * @return long the result hash
 */
long polynomial_rolling_hash(char* key) {
    const int p = 173; // prime number near ASC2 character table size
    const int m = 1e9 +
                  9; // This is a large number, but still small enough so that we can perform multiplication of two values using 64 bit integers.
    unsigned long hash_value = 0;
    long long p_pow = 1;

    char* aux = key;
    while(*aux != '\0') {
        hash_value = (hash_value + (*aux - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
        aux++; //Move to the next char
    }

    return hash_value;

}

long simple_hash(char* key) {
    long hash = 0;
    char* aux = key;

    while(*aux != '\0') {
        hash = 31*hash + *aux;
        aux++; //Move to the next char
    }

    return hash;
}

struct hash_table* hash_table_create(int length) {

    struct hash_table* ht = malloc(sizeof(struct hash_table));
    ht->table = malloc(sizeof(struct hash_table_list) * length);

    //Initialize the array
    int i = 0;
    while(i++ < length) ht->table[i] = NULL;

    ht->length = length;

    return ht;
}

void hash_table_insert(struct hash_table* hashTable, char* key, struct document* doc, hash_function hash) {
    int pos = hash(key) % hashTable->length;

    struct hash_table_list* new = malloc(sizeof(struct hash_table_list));
    new->doc = doc;
    new->next = hashTable->table[pos];
    hashTable->table[pos] = new;
}
