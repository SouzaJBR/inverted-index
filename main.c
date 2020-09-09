#include <stdio.h>
#include <string.h>

#include "io/filereader.h"
#include "io/parser.h"
#include "structure/hash.h"
#include "structure/trie.h"


int main() {
    printf("Hello, World!\n");

    struct file_reader* fr = file_reader_create("/home/souzajbr/codez/trabalho-aed/data/News_Category_Dataset_v2.json");

    //struct trie* trie = trie_create();
    struct hash_table* hashTable = hash_table_create(HASH_TABLE_SIZE, HASH_FUNCTION_SIMPLE);


    int i = 0;
    while(i++ < 10000) {

        char* token;
        struct document* doc = file_reader_get_next_document(fr);
        struct parser* parserHeadline = parser_create(doc->headline);
        struct parser* parserDescription = parser_create(doc->shortDescription);
        while((token = parser_get_next_token(parserHeadline)) != NULL) {
            //trie_insert(trie, token, doc);
            hash_table_insert(hashTable, token, doc);
        }
        while((token = parser_get_next_token(parserDescription)) != NULL) {
            //trie_insert(trie, token, doc);
            hash_table_insert(hashTable, token, doc);
        }

    }

    return 0;
}
