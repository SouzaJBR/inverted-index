//
// Created by souzajbr on 08/09/2020.
//

#ifndef TRABALHO_AED_FILEREADER_H
#define TRABALHO_AED_FILEREADER_H

#include <stdlib.h>
#include <stdio.h>

#include "../structure/document.h"
#include "../deps/cJSON/cJSON.h"

typedef void (*insert_function)(void*,char*,struct document*);

struct file_reader {
    cJSON* root;
    cJSON* current;
    char* path;
};

struct file_reader* file_reader_create(char* path);
void file_reader_destroy(struct file_reader* fr);
struct document* file_reader_get_next_document(struct file_reader* fr);
int file_reader_load_documents(struct file_reader* fr, void* structure, insert_function insert);

#endif //TRABALHO_AED_FILEREADER_H
