//
// Created by souzajbr on 09/09/2020.
//


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "parser.h"

#define ACCENT_CHARS    "ÁÀÃÂÇáàãâçÉÊéêÍíÑÓÔÕñóôõÚÜúü"
#define UNACCENT_CHARS  "AAAACaaaacEEeeIiNOOOnoooUUuu"

wchar_t buffer[4096];
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

bool parser_is_stopword(char *word) {

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

    if(iswblank(c) != 0 || iswcntrl(c) != 0)
        return true;

    return false;
}

bool parser_is_skip_char(char c) {

    if(ispunct(c) != 0)
        return true;

    return false;
}

wchar_t parser_normalize_char(wchar_t c) {

    if(iswdigit(c))
        return c;

    c = towlower(c);

    switch (c) {
        case L'a':
        case L'b':
        case L'c':
        case L'd':
        case L'e':
        case L'f':
        case L'g':
        case L'h':
        case L'i':
        case L'j':
        case L'k':
        case L'l':
        case L'm':
        case L'n':
        case L'o':
        case L'p':
        case L'q':
        case L'r':
        case L's':
        case L't':
        case L'u':
        case L'v':
        case L'w':
        case L'x':
        case L'y':
        case L'z':
            return c;
        case L'á':
        case L'ã':
        case L'à':
        case L'â':
        case L'ä':
        case L'å':
        case L'æ':
            return 'a';
        case L'ç':
            return 'c';
        case L'é':
        case L'è':
        case L'ê':
        case L'ë':
            return L'e';
        case L'ì':
        case L'í':
        case L'î':
        case L'ï':
            return L'i';
        case L'ò':
        case L'ô':
        case L'ó':
        case L'ö':
        case L'õ':
            return L'o';
        default:
            return L' ';
    }

}

struct parser *parser_create(wchar_t *input) {
    struct parser *new = malloc(sizeof(struct parser));
    new->input = input;
    new->pos = 0;

    return new;
}

void parser_destroy(struct parser* parser) {
    free(parser);
}

char * parser_get_next_token(struct parser *parser) {

    bufferPos = 0;


    while (true) {
        wchar_t c = parser->input[parser->pos++];

        if (parser_is_separator(c)) { //if is a separator, return the token found
            if (bufferPos > 0) {
                buffer[bufferPos] = L'\0';
                char *token = malloc(sizeof(char) * (bufferPos + 1));

                wcstombs(token, buffer, bufferPos + 1);
                return token;
            }
            if (c == '\0') //EOS
                return NULL;
        }

        c = parser_normalize_char(c);

        if(c == L' ')
            continue;

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