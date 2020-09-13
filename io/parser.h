//
// Created by souzajbr on 09/09/2020.
//

#ifndef TRABALHO_AED_PARSER_H
#define TRABALHO_AED_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <wctype.h>
#include <wchar.h>

struct parser {
    wchar_t *input;
    int pos;
};

struct parser *parser_create(wchar_t *input);

wchar_t *parser_get_next_token(struct parser *parser);

bool parser_is_stopword(wchar_t *word);

#endif //TRABALHO_AED_PARSER_H
