//
// Created by souzajbr on 08/09/2020.
//

#include "filereader.h"


struct file_reader* file_reader_create(char* path) {
    struct file_reader* fr = (struct file_reader*) malloc(sizeof(struct file_reader));

    FILE *f = fopen(path, "rb");

    // Get the file size
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read all its content
    char* content = malloc(fileSize + 3); // 3 = '\0', '['  e ']'
    content[0] = '[';
    fread(content+1, 1, fileSize, f);
    fclose(f);

    for(long i = 0; i < fileSize; i++)
        if(content[i] == '\n') content[i] = ',';

    content[fileSize-1] = ']';
    content[fileSize] = 0; //NULL-termination string

    fr->root = cJSON_Parse(content);
    fr->current = fr->root->child;
    fr->path = path;

    return fr;
}

struct document* file_reader_get_next_document(struct file_reader* fr) {

    cJSON* json = fr->current;
    cJSON *category = cJSON_GetObjectItem(json, "category");
    cJSON *headline = cJSON_GetObjectItem(json, "headline");
    cJSON *link = cJSON_GetObjectItem(json, "link");
    cJSON *authors = cJSON_GetObjectItem(json, "authors");
    cJSON *short_description = cJSON_GetObjectItem(json, "short_description");

    struct document* doc = document_create(headline->valuestring, short_description->valuestring, link->valuestring);

    fr->current = json->next;

    return doc;
}