//
// Created by souzajbr on 09/09/2020.
//

#ifndef TRABALHO_AED_RANK_H
#define TRABALHO_AED_RANK_H

#include "structure/document.h"
#include "structure/vector.h"
#include "io/parser.h"
#include "math.h"

struct document_relevancy {
    char* token;
    struct document* document;
    double relevancy;
};

struct vector* rank_get_keyword_weights(struct document_list* list, char * keyword, int total_documents);
int rank_get_document_unique_tokens(struct document* doc);
void rank_sort_results(struct vector* query);
struct vector* rank_join_results(struct vector* query1, struct vector* query2);
void rank_quicksort_swap(struct document_relevancy** v1, struct document_relevancy** v2);

#endif //TRABALHO_AED_RANK_H
