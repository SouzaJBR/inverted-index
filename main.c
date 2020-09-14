#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "io/filereader.h"
#include "io/parser.h"
#include "structure/hash.h"
#include "structure/trie.h"
#include "structure/vector.h"
#include "rank.h"
#include "stdbool.h"

#include <math.h>
#include "tests/test_hash.h"

#define ascii_divider "`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'\n   `     `     `     `     `     `     `     `     `     `"


typedef struct document_list* (*search_function) (void*, char*);


bool select_structure() {

    wprintf(L"\nSelect a type:\n\t1 - Hash\n\t2 - Trie\n> ");
    char c = (char) getchar();
    getchar(); //Capture <ENTER>

    switch (c) {
        case '1':
            return true;
        case '2':
            return false;
        default:
            wprintf(L"\n[ERROR] Invalid option\n");
            return select_structure();
    }
}

void search_console(void* structure, search_function search, int totalDocuments, int maxResults) {

    wprintf(L"\nConsole loaded with %d documents.\n Example query:\n\tmusic\n\tcomputer science\n\tmovie star\n", totalDocuments);

    while(true) {

       wprintf(L"Type your search query: \n(Ctrl-C to exit)\n> ");

        char buffer[256];
        wchar_t input[256];

        fgets(buffer, 256, stdin);
        mbstowcs(input, buffer, 256);

        if(wcscasecmp(L"^C", input) == 0)
            break;

        struct parser* parser = parser_create(input);

        char* keyword1 = parser_get_next_token(parser);
        char* keyword2 = parser_get_next_token(parser);

        if(keyword1 == NULL && keyword2 == NULL) {
            fwprintf(stderr, L"\n[ERROR] Please type a search query\n\n");
            continue;
        }

        struct document_list* l1 = search(structure, keyword1);
        struct document_list* l2 = search(structure, keyword2);

        struct vector* rel1 = rank_get_keyword_weights(l1, keyword1, totalDocuments);
        struct vector* rel2 = rank_get_keyword_weights(l2, keyword1, totalDocuments);

        struct vector* rel = rank_join_results(rel1, rel2);

        rank_sort_results(rel);

        int count = 0;

        wprintf(L"\n\n%s\n%s\nBest %d results from %d\n\n", ascii_divider, ascii_divider, rel->length > maxResults ? maxResults : rel->length , rel->length);

        while(count < rel->length && count <= maxResults){
            struct document_relevancy* dr = ((struct document_relevancy**) (rel->array))[count++];
            wprintf(L"--------\n%ls (%s)\n%ls\n------\n\n", dr->document->headline, dr->document->link, dr->document->shortDescription);

        }

        wprintf(L"\n\n");
    }
}

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "");

    if(argc < 2) {
        wprintf(L"USAGE: %s <json_input>\n", argv[0]);
        return 1;
    }


    wprintf(L"Reading input file...\n");
    struct file_reader* fr = file_reader_create(argv[1]);

    if(fr == NULL) {
        wprintf(L"Cannot open the input file. Check if the file exists or it has correct format.\n");
        return 2;
    }

    bool useHash = select_structure();

    if(useHash) {
        struct hash_table* hashTable = hash_table_create(HASH_TABLE_SIZE, HASH_FUNCTION_SIMPLE);

        int total = file_reader_load_documents(fr, hashTable, (insert_function) &hash_table_insert);

        search_console(hashTable, (search_function) &hash_table_search, total, 20);

    } else {
        struct trie* trie = trie_create();
        int total = file_reader_load_documents(fr, trie, (insert_function) &trie_insert);
        search_console(trie, (search_function) &trie_search, total, 20);
    }

    return 0;

}
