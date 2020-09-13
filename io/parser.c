//
// Created by souzajbr on 09/09/2020.
//


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "parser.h"

#define ACCENT_CHARS    "ÁÀÃÂÇáàãâçÉÊéêÍíÑÓÔÕñóôõÚÜúü"
#define UNACCENT_CHARS  "AAAACaaaacEEeeIiNOOOnoooUUuu"

wchar_t buffer[256];
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

bool parser_is_stopword(wchar_t *word) {

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

bool parser_is_separator(wchar_t c) {


    if(iswblank(c) != 0 || c == L'\0')
        return true;

    return false;
}

bool parser_is_skip_char(char c) {

    if(ispunct(c) != 0)
        return true;

    return false;
}

wchar_t parser_normalize_char(wchar_t c) {



    return c;
}

struct parser *parser_create(wchar_t *input) {
    struct parser *new = malloc(sizeof(struct parser));
    new->input = input;
    new->pos = 0;

    return new;
}

wchar_t * parser_get_next_token(struct parser *parser) {

    bufferPos = 0;


    while (true) {
        wchar_t c = parser->input[parser->pos++];

        if (parser_is_separator(c)) { //if is a separator, return the token found
            if (bufferPos > 0) {
                buffer[bufferPos] = L'\0';
                wchar_t *token = malloc(sizeof(wchar_t) * (bufferPos + 1));
                wcscpy(token, buffer);
                return token;
            }
            if (c == '\0') //EOS
                return NULL;
        }

        c = parser_normalize_char(c);
        if (bufferPos == 0) //tokens should start with letters
        {
            if (!iswalpha(c)) {
                while (!parser_is_separator((c = parser->input[parser->pos++]))) continue;

                if (c == L'\0') return NULL;

                continue;
            }
            buffer[bufferPos++] = c;
        } else {
            if (!parser_is_skip_char(c))
                buffer[bufferPos++] = c;
        }


    }
}