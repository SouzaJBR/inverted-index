//
// Created by souzajbr on 08/09/2020.
//


#include "stdbool.h"

#include "../structure/trie.h"
#include "../structure/hash.h"

#include <locale.h>

#include <stdio.h>
#include "filereader.h"
#include "wchar.h"
#include "parser.h"

struct file_reader *file_reader_create(char *path) {
    struct file_reader *fr = (struct file_reader *) malloc(sizeof(struct file_reader));

    FILE *f = fopen(path, "rb");

    if (!f)
        return NULL;

    // Get the file size
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read all its content
    char *content = (char *) malloc(fileSize + 3); // 3 = '\0', '['  e ']'

    /* The dataset file format is not a valid JSON array.
     * It contains a document object per line.
     * So, before using the JSON parser, we need to correct it
     * with array tokens and replacing new-lines with a comma 
     */

    content[0] = '[';
    size_t totalChars = fread(content + 1, sizeof(char), fileSize, f);

    if((totalChars + 1) * sizeof(char) < fileSize)
        return NULL;

    fclose(f);

    // Array separation
    for (long i = 0; i < fileSize; i++)
        if (content[i] == '\n') content[i] = ',';

    content[fileSize - 1] = ']'; // Close array token
    content[fileSize] = 0; //NULL-termination string

    fr->root = cJSON_Parse(content);
    fr->current = fr->root->child;
    fr->path = path;

    return fr;
}

void file_reader_destroy(struct file_reader *fr) {
    cJSON_Delete(fr->root);
    free(fr);
}

struct document *file_reader_get_next_document(struct file_reader *fr) {

    cJSON *json = fr->current;

    if (json == NULL)
        return NULL;

    cJSON *category = cJSON_GetObjectItem(json, "category");
    cJSON *headline = cJSON_GetObjectItem(json, "headline");
    cJSON *link = cJSON_GetObjectItem(json, "link");
    cJSON *authors = cJSON_GetObjectItem(json, "authors");
    cJSON *short_description = cJSON_GetObjectItem(json, "short_description");


    struct document *doc = document_create(headline->valuestring, short_description->valuestring, link->valuestring);

    fr->current = json->next;

    return doc;
}

int file_reader_load_documents(struct file_reader *fr, void *structure, insert_function insert) {
    int i = 0;

    while (true) {
        if (i % 10000 == 0)
            wprintf(L"Inserted %d documents.\n", i);

        char *token;
        struct document *doc = file_reader_get_next_document(fr);

        if (doc == NULL)
            break;

        i++;

        struct parser *parserHeadline = parser_create(doc->headline);
        struct parser *parserDescription = parser_create(doc->shortDescription);
        while ((token = parser_get_next_token(parserHeadline)) != NULL) {

            if (!parser_is_stopword(token))
                insert(structure, token, doc);

        }
        while ((token = parser_get_next_token(parserDescription)) != NULL) {
            if (!parser_is_stopword(token))
                insert(structure, token, doc);

        }
    }

    return i;
}