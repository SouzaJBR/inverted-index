//
// Created by souzajbr on 09/09/2020.
//


#include <string.h>
#include <stdio.h>
#include "parser.h"

char buffer[256];
int bufferPos = 0;

bool parser_is_number(char c) {
    return c >= 48 && c <=57;
}

bool parser_is_letter(char c) {
    return c >= 97 && c <= 122;
}
bool parser_is_separator(char c) {
    switch(c) {
        case '\n':
        case '\t':
        case '\0':
        case ' ':
        case '-':
            return true;
        default:
            return false;
    }
}

bool parser_is_skip_char(char c) {

    switch(c) {
        case '.':
        case ',':
        case '!':
        case '?':
        case '\'':
        case '\"':
        case '`':
            return true;
        default:
            return false;
    }
}

char parser_normalize_char(char c) {

    if(c>= 65 && c <= 90) // toLowerCase
        return c += 32;

    if(c == "à")
        return 'a';
    return c;
}

struct parser* parser_create(char* input) {
    struct parser* new = malloc(sizeof(struct parser));
    new->input = input;
    new->pos = 0;

    return new;
}

char* parser_get_next_token(struct parser* parser) {

    bufferPos = 0;


    while(true) {
        char c = parser->input[parser->pos++];

        if (parser_is_separator(c)) { //if is a separator, return the token found
            if (bufferPos > 0) {
                buffer[bufferPos] = '\0';
                char *token = malloc(bufferPos+1);
                strcpy(token, buffer);
                return token;
            }
            if (c == '\0') //EOS
                return NULL;
        }

        c = parser_normalize_char(c);
        if(bufferPos == 0) //tokens should start with letters
        {
            if(!parser_is_letter(c))
                continue;

            buffer[bufferPos++] = c;
        }
        else {
            if(!parser_is_skip_char(c))
                buffer[bufferPos++] = c;
        }


    }
}