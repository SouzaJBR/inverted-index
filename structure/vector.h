//
// Created by souzajbr on 09/09/2020.
//

#ifndef TRABALHO_AED_VECTOR_H
#define TRABALHO_AED_VECTOR_H

#include <stdbool.h>
#define VECTOR_ARRAY_SIZE 2048
typedef bool (*vector_compare)(void*,void*);

struct vector {
    void** array;
    int length;
    int arraySize;
    vector_compare compare;
};

struct vector* vector_create(vector_compare compare);
void vector_insert(struct vector* vector, void* object);
bool vector_contains(struct vector* vector, void* object);
bool vector_compare_string(void* c1, void* c2);
bool vector_compare_doc_relev(void* c1, void* c2);
void* vector_search(struct vector* vector, void* object);
void vector_destroy(struct vector* vector);


#endif //TRABALHO_AED_VECTOR_H
