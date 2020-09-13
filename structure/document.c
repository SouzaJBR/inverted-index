//
// Created by souzajbr on 08/09/2020.
//

#include "hash.h"
#include <string.h>
#include "document.h"
#include "stdlib.h"

int current_document_id = 1;

struct document *document_create(char *headline, char *shortDescription, char *link) {

    struct document* doc = (struct document*) malloc(sizeof(struct document));

    size_t headlineLength = strlen(headline) + 1;
    size_t shortDescriptionLength = strlen(shortDescription) + 1;

    doc->id = current_document_id++;
    doc->headline = malloc(sizeof(wchar_t) * headlineLength);
    doc->shortDescription = malloc(sizeof(wchar_t) * shortDescriptionLength);
    doc->link = malloc(strlen(link) + 1);
    doc->distinctTokens = -1;

    mbstowcs(doc->headline, headline, headlineLength);
    mbstowcs(doc->shortDescription, shortDescription, shortDescriptionLength);
    strcpy(doc->link, link);

    return doc;

}

struct document_list *document_list_create(struct document *doc) {
    struct document_list* new = (struct document_list*) malloc(sizeof(struct document_list));
    new->next = NULL;
    new->doc = doc;
    new->count = 1;

    return new;
}

struct document_list* document_list_insert_aux(struct document_list* root, struct document* doc) {

    if(root == NULL) {
        return document_list_create(doc);

    }

    if(root->doc->id < doc->id)
        root->next = document_list_insert_aux(root->next, doc);
    else if(root->doc->id == doc->id)
        root->count++;
    else if (root->doc->id > doc->id) {
        struct document_list* new = document_list_create(doc);
        new->next = root;
        return new;
    }

    return root;

}