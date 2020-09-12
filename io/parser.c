//
// Created by souzajbr on 09/09/2020.
//


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "parser.h"

char buffer[256];
int bufferPos = 0;

char *stopwords[127] = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself",
                        "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its",
                        "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom",
                        "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being",
                        "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but",
                        "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about",
                        "against", "between", "into", "through", "during", "before", "after", "above", "below", "to",
                        "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then",
                        "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few",
                        "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so",
                        "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};

bool parser_is_stopword(char* word) {

    for(int i = 0; i < 127; i++) {
        if(strcmp(word, stopwords[i]) == 0)
            return true;
    }

    return false;
}

bool parser_is_number(char c) {
    return c >= 48 && c <= 57;
}

bool parser_is_letter(char c) {
    return c >= 97 && c <= 122;
}

bool parser_is_separator(char c) {


    if(isblank(c) != 0 || c == '\0')
        return true;

    return false;
}

bool parser_is_skip_char(char c) {

    if(ispunct(c) != 0)
        return true;

    return false;
}

char parser_normalize_char(char c) {

    if (c >= 65 && c <= 90) // toLowerCase
        return c += 32;

    if (c == "à")
        return 'a';
    return c;
}

struct parser *parser_create(char *input) {
    struct parser *new = malloc(sizeof(struct parser));
    new->input = input;
    new->pos = 0;

    return new;
}

char *parser_get_next_token(struct parser *parser) {

    bufferPos = 0;


    while (true) {
        char c = parser->input[parser->pos++];

        if (parser_is_separator(c)) { //if is a separator, return the token found
            if (bufferPos > 0) {
                buffer[bufferPos] = '\0';
                char *token = malloc(bufferPos + 1);
                strcpy(token, buffer);
                return token;
            }
            if (c == '\0') //EOS
                return NULL;
        }

        c = parser_normalize_char(c);
        if (bufferPos == 0) //tokens should start with letters
        {
            if (!parser_is_letter(c))
                continue;

            buffer[bufferPos++] = c;
        } else {
            if (!parser_is_skip_char(c))
                buffer[bufferPos++] = c;
        }


    }
}