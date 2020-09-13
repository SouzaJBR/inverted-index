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


bool useHash = true;


void initialize() {

}

int main() {

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
//    }

//    return 0;
    char* path = "/home/souzajbr/codez/inverted-index/data/dataset.json";

    FILE *f = fopen(path, "rb");

    fseek(f, 0, SEEK_END);
    int fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    printf("File Size: %ld bytes\n", fileSize);

    int i = 0;


    wchar_t * content = calloc(sizeof(wchar_t) * (fileSize + 3), sizeof(wchar_t)); // 3 = '\0', '['  e ']'
    char * buffer = calloc(sizeof(wchar_t) * (fileSize + 3), sizeof(char ));



    buffer[0] = '[';
    fread(buffer + 1,sizeof(char*), (int) fileSize, f);
    fclose(f);

//    for(long i = 0; i < fileSize; i++)
//        if(content[i] == '\n') content[i] = ',';

    buffer[fileSize-1] = L']';
    buffer[fileSize] = 0; //NULL-termination string

    size_t ss = mbstowcs(content, buffer, fileSize);

    printf("-\n%s\n-\n", content);

    return 0;
}

bool select_structure() {

    printf("\nSelecione uma estrutura:\n\t1 - Hash\n\t2 - Trie\n>");
    char c = (char) getchar();
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

int main2(int argc, char *argv[]) {



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
