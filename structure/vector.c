//
// Created by souzajbr on 09/09/2020.
//

#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "../rank.h"

bool vector_compare_string(void* c1, void* c2) {
    return strcmp((char*) c1, (char *) c2) == 0;
}

bool vector_compare_doc_relev(void* c1, void* c2) {
    return ((struct document_relevancy*) c1)->document->id == ((struct document_relevancy*) c2)->document->id;
}

struct vector* vector_create(vector_compare compare) {

    struct vector* new = malloc(sizeof(struct vector));

    new->array = malloc(sizeof(void *) * VECTOR_ARRAY_SIZE);
    new->length = 0;
    new->arraySize = VECTOR_ARRAY_SIZE;
    new->compare = compare;
}

void vector_destroy(struct vector* vector) {
    free(vector->array);
    free(vector);
}
void vector_insert(struct vector* vector, void* object) {

    if(vector->length + 1 >= vector->arraySize)
    {
        vector->arraySize += VECTOR_ARRAY_SIZE;
        vector->array = realloc(vector->array, sizeof(void *) * vector->arraySize);
    }

    vector->array[vector->length++] = object;
}
bool vector_contains(struct vector* vector, void* object) {
    return vector_search(vector, object) != NULL;
}

void* vector_search(struct vector* vector, void* object) {

    for(int i = 0; i < vector->length; i++) {
        if(vector->compare(vector->array[i], object))
            return vector->array[i];
    }

    return NULL;
}