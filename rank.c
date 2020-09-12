//
// Created by souzajbr on 09/09/2020.
//

#include "rank.h"

void rank_quicksort_swap(struct document_relevancy** v1, struct document_relevancy** v2){
    struct document_relevancy* tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

int rank_quicksort_partition(struct document_relevancy* arr[], int low, int high) {
    double pivot = arr[high]->relevancy;    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j]->relevancy > pivot)
        {
            i++;    // increment index of smaller element
            rank_quicksort_swap(&arr[i], &arr[j]);
        }
    }
    rank_quicksort_swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void rank_quicksort(struct document_relevancy* arr[], int low, int high) {
    if (low < high)
    {
        int pi = rank_quicksort_partition(arr, low, high);
        rank_quicksort(arr, low, pi - 1);
        rank_quicksort(arr, pi + 1, high);
    }

}

int rank_get_document_unique_tokens(struct document* doc) {

    struct parser* pHead = parser_create(doc->headline);
    struct parser* pDesc = parser_create(doc->shortDescription);

    struct vector* vector = vector_create(&vector_compare_string);

    char* aux;
    while((aux = parser_get_next_token(pHead)) != NULL)
        if(!vector_contains(vector, aux))
            vector_insert(vector, aux);

    while((aux = parser_get_next_token(pDesc)) != NULL)
        if(!vector_contains(vector, aux))
            vector_insert(vector, aux);

    return vector->length;
}

int rank_get_total_documents_containing(struct document_list* list) {
    int count = 0;

    while(list != NULL)
    {
        list = list->next;
        count++;
    }

    return count;
}

void rank_sort_results(struct vector* query) {
    rank_quicksort((struct document_relevancy**)query->array, 0, query->length-1);
}

struct vector* rank_join_results(struct vector* query1, struct vector* query2) {
    struct vector* result = vector_create(&vector_compare_doc_relev);

    int pos1 = 0, pos2 = 0;
    while(pos1 < query1->length || pos2 < query2->length) {
        struct document_relevancy* d1 = query1->array[pos1];
        struct document_relevancy* d2 = query2->array[pos2];


        if(pos2 >= query2->length || d1->document->id < d2->document->id) {
            vector_insert(result, d1);
            pos1++;
        } else if(pos1 >= query1->length || d1->document->id > d2->document->id) {
            vector_insert(result, d2);
            pos2++;
        } else {
            struct document_relevancy* new = malloc(sizeof(struct document_relevancy));
            new->document = d1->document;
            new->relevancy = d1->relevancy + d2->relevancy;
            new->token = "<both>";
            vector_insert(result, new);
            pos1++;
            pos2++;
        }
    }

    return result;
}

struct vector* rank_get_keyword_weights(struct document_list* list, char * keyword, int total_documents) {

    int document_count = 0;
    int total_documents_containing = rank_get_total_documents_containing(list); //dj

    struct vector* docs = vector_create(vector_compare_doc_relev);

    while(list != NULL) {

        document_count++;
        int unique_tokens = list->doc->distinctTokens;

        if(unique_tokens < 0) {
            unique_tokens = rank_get_document_unique_tokens(list->doc);
            list->doc->distinctTokens = unique_tokens;
        }

        int token_freq_in_doc = list->count; //fij

        double weight = token_freq_in_doc * (log10(total_documents)/total_documents_containing);
        double relevancy = ((double)1/unique_tokens) * weight;

        struct document_relevancy* doc_rel = malloc(sizeof(struct document_relevancy));
        doc_rel->document = list->doc;
        doc_rel->token = keyword;
        doc_rel->relevancy = relevancy;

        vector_insert(docs, doc_rel);
        list = list->next;
    }

    return docs;
}