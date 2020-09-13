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

#define ascii_divider "`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'`'. .'\n   `     `     `     `     `     `     `     `     `     `"

typedef void (*insert_function)(void*,char*,struct document*);
typedef struct document_list* (*search_function) (void*, char*);


void initialize() {

}

int main4() {
    wchar_t * text = L"Aço vindo Òòóóó1oòóóó da Nicarágua é tão doido que vira pó";
    struct parser* p = parser_create(text);

    char* token;
    while((token = parser_get_next_token(p)) != NULL) {
        printf("-%s-\n", token);
    }
}

int main3() {

    setlocale(LC_ALL, "");
//    char *str = "çhis is tutorialspoint.com\nÇa va bien";
//
//    wchar_t* x = calloc(100, sizeof(wchar_t));
//    char* y = calloc(100, sizeof(char ));
//
//
//    size_t ss = mbstowcs(x, str, 100);
//
//    if(x[0] == L'ç') {
//
////    }
//
////    return 0;
//    char* path = "/home/souzajbr/codez/inverted-index/data/dataset.json";
//
//    FILE *f = fopen(path, "rb");
//
//    fseek(f, 0, SEEK_END);
//    int fileSize = ftell(f);
//    fseek(f, 0, SEEK_SET);
//
//    printf("File Size: %ld bytes\n", fileSize);
//
//    int i = 0;
//
//
//    wchar_t * content = calloc(sizeof(wchar_t) * (fileSize + 3), sizeof(wchar_t)); // 3 = '\0', '['  e ']'
//    char * buffer = calloc(sizeof(wchar_t) * (fileSize + 3), sizeof(char ));
//
//
//
//    buffer[0] = '[';
//    fread(buffer + 1,sizeof(char*), (int) fileSize, f);
//    fclose(f);
//
////    for(long i = 0; i < fileSize; i++)
////        if(content[i] == '\n') content[i] = ',';
//
//    buffer[fileSize-1] = L']';
//    buffer[fileSize] = 0; //NULL-termination string
//
//    size_t ss = mbstowcs(content, buffer, fileSize);
//
//    printf("-\n%s\n-\n", content);

    return 0;
}

bool select_structure() {

    wprintf(L"\nSelecione uma estrutura:\n\t1 - Hash\n\t2 - Trie\n> ");
    char c = (char) getchar();
    getchar(); //Capture <ENTER>

    switch (c) {
        case '1':
            return true;
        case '2':
            return false;
        default:
            wprintf(L"\nOpção inválida\n");
            return select_structure();
    }
}

int load_documents(struct file_reader* fr, void* structure,insert_function insert) {
    int i = 0;

    while(i < 30000) {
        if(i % 10000 == 0)
            wprintf(L"Inserted %d documents.\n", i);

        char* token;
        struct document* doc = file_reader_get_next_document(fr);

        if(doc == NULL)
            break;

        i++;

        struct parser* parserHeadline = parser_create(doc->headline);
        struct parser* parserDescription = parser_create(doc->shortDescription);
        while((token = parser_get_next_token(parserHeadline)) != NULL) {

            if(!parser_is_stopword(token))
                insert(structure, token, doc);

        }
        while((token = parser_get_next_token(parserDescription)) != NULL) {
            if(!parser_is_stopword(token))
                insert(structure, token, doc);

        }
    }

    return i;
}

void search_console(void* structure, search_function search, int totalDocuments, int maxResults) {


    while(true) {

        wprintf(L"Console loaded with %d documents.\nType your search query:\n> ", totalDocuments);

        char buffer[256];
        wchar_t input[256];

        fgets(buffer, 256, stdin);
        mbstowcs(input, buffer, 256);

        if(wcscasecmp(L"\\quit", input) == 0)
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
        wprintf(L"USO: %s <json_entrada>\n", argv[0]);
        return 1;
    }


    wprintf(L"Lendo arquivo de entrada...\n");
    struct file_reader* fr = file_reader_create(argv[1]);

    if(fr == NULL) {
        wprintf(L"Não foi possível abrir o arquivo. Verifique se o caminho existe\n");
        return 2;
    }

    bool useHash = select_structure();

    if(useHash) {
        struct hash_table* hashTable = hash_table_create(HASH_TABLE_SIZE, HASH_FUNCTION_SIMPLE);

        int total = load_documents(fr, hashTable, (insert_function) &hash_table_insert);

        search_console(hashTable, (search_function) &hash_table_search, total, 20);
        return 0;
    }

    struct trie* trie = trie_create();



    int i = 0;
    while(true) {

        if(i++ % 10000 == 0)
            wprintf(L"Inserted %d documents.\n", i);

        char* token;
        struct document* doc = file_reader_get_next_document(fr);

        if(doc == NULL)
            break;

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

    file_reader_destroy(fr);

    //struct document_list* list1 = hash_table_search(hashTable, "trump");
    struct document_list* list1 = trie_search(trie, "trump");
    struct vector* rel1 = rank_get_keyword_weights(list1, "trump", i);

    //struct document_list* list2 = hash_table_search(hashTable, "tremendous");
    struct document_list* list2 = trie_search(trie, "melania");
    struct vector* rel2 = rank_get_keyword_weights(list2, "tremendous", i);

    struct vector* rel = rank_join_results(rel1, rel2);




    rank_sort_results(rel);

    i = 0;


    while(i < rel->length){
        struct document_relevancy* dr = ((struct document_relevancy**) (rel->array))[i++];
        fwprintf(stdout, L"%ls (%s)\n%ls\n\n", dr->document->headline, dr->document->link, dr->document->shortDescription);

    }


    return 0;
}
