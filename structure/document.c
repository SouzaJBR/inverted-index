//
// Created by souzajbr on 08/09/2020.
//

#include <string.h>
#include "document.h"
#include "stdlib.h"

int current_document_id = 1;

struct document *document_create(char *headline, char *shortDescription, char *link) {

    struct document* doc = (struct document*) malloc(sizeof(struct document));

    doc->id = current_document_id++;
    doc->headline = malloc(strlen(headline) + 1);
    doc->shortDescription = malloc(strlen(shortDescription) + 1);
    doc->link = malloc(strlen(link) + 1);

    strcpy(doc->headline, headline);
    strcpy(doc->shortDescription, shortDescription);
    strcpy(doc->link, link);

    return doc;

}