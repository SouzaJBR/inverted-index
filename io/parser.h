//
// Created by souzajbr on 09/09/2020.
//

#ifndef TRABALHO_AED_PARSER_H
#define TRABALHO_AED_PARSER_H

#include <stdlib.h>
#include <stdbool.h>

struct parser {
    char * input;
    int pos;
};

struct parser* parser_create(char* input);
char* parser_get_next_token(struct parser* parser);
bool parser_is_stopword(char* word);

#endif //TRABALHO_AED_PARSER_H
