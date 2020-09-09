//
// Created by souzajbr on 07/09/2020.
//

#ifndef TRABALHO_AED_DOCUMENT_H
#define TRABALHO_AED_DOCUMENT_H


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct document {
    int id;
    char* headline;
    char* shortDescription;
    char* url;
    char* link;
};

struct document_list {
    struct document_list* next;
    struct document* doc;
    int count;
};

struct document* document_create(char* headline, char* shortDescription, char* link);
struct document_list *document_list_create(struct document *doc);
struct document_list* document_list_insert_aux(struct document_list* root, struct document* doc);

#endif //TRABALHO_AED_DOCUMENT_H
