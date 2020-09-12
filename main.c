#include <stdio.h>
#include <string.h>

#include "io/filereader.h"
#include "io/parser.h"
#include "structure/hash.h"
#include "structure/trie.h"
#include "structure/vector.h"
#include "rank.h"
#include "stdbool.h"

bool useHash = true;


void initialize() {

}

bool select_structure() {

    printf("\nSelecione uma estrutura:\n\t1 - Hash\n\t2 - Trie\n>");
    char c = getchar();
    getchar(); //Capture <ENTER>

    switch (c) {
        case '1':
            return true;
        case '2':
            useHash = false;
            return false;
        default:
            printf("\nOpção inválida\n");
            return select_structure();
    }
}

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("USO: %s <json_entrada>\n", argv[0]);
        return 1;
    }

    printf("Lendo arquivo de entrada...\n");
    struct file_reader* fr = file_reader_create(argv[1]);

    if(fr == NULL) {
        printf("Não foi possível abrir o arquivo. Verifique se o caminho existe\n");
        return 2;
    }

    printf("%d\n", select_structure());
    return 0;
    struct trie* trie = trie_create();
    struct hash_table* hashTable = hash_table_create(HASH_TABLE_SIZE, HASH_FUNCTION_SIMPLE);


    int i = 0;
    while(i++ < 10000) {

        char* token;
        struct document* doc = file_reader_get_next_document(fr);
        struct parser* parserHeadline = parser_create(doc->headline);
        struct parser* parserDescription = parser_create(doc->shortDescription);
        while((token = parser_get_next_token(parserHeadline)) != NULL) {

            if(!parser_is_stopword(token))
                trie_insert(trie, token, doc);
                //hash_table_insert(hashTable, token, doc);
        }
        while((token = parser_get_next_token(parserDescription)) != NULL) {
            if(!parser_is_stopword(token))
                trie_insert(trie, token, doc);
                //hash_table_insert(hashTable, token, doc);
        }

        //printf("%d\n", doc->id);

    }

    struct document_list* list1 = hash_table_search(hashTable, "trump");
    struct document_list* list2 = hash_table_search(hashTable, "melania");

    struct vector* rel1 = rank_get_keyword_weights(list1, "trump", 10000);
    struct vector* rel2 = rank_get_keyword_weights(list2, "melania", 10000);


    struct vector* rel = rank_join_results(rel1, rel2);

    rank_sort_results(rel);

    i = 0;
    while(i < 20){
        struct document_relevancy* dr = ((struct document_relevancy**) (rel->array))[i++];
        printf("[%f] %s (%s)\n%s\n\n", dr->relevancy, dr->document->headline, dr->document->link, dr->document->shortDescription);
    }

    return 0;
}
