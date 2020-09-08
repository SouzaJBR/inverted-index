//
// Created by souzajbr on 07/09/2020.
//

#ifndef TRABALHO_AED_DOCUMENT_H
#define TRABALHO_AED_DOCUMENT_H


struct document {
    int id;
    char* headline;
    char* shortDescription;
    char* url;
    char* link;
};

struct document* document_create(char* headline, char* shortDescription, char* link);

#endif //TRABALHO_AED_DOCUMENT_H
