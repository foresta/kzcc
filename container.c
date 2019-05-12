#include "kzcc.h"

//
// Vector
//

Vector *new_vector() {
    int initial_capacity = 16;
    Vector *vec = malloc(sizeof(Vector));
    vec->data = malloc(sizeof(void *) * initial_capacity);
    vec->capacity = initial_capacity;
    vec->length = 0;
    return vec;
}

void vec_push(Vector *vec, void *element) {
    if (vec->capacity == vec->length) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }
    vec->data[vec->length++] = element;
}


// Test

void test_vector() {
    Vector *vec = new_vector();
    expect(__LINE__, 0, vec->length);

    for (int i = 0; i < 100; i++)
        vec_push(vec, (void *)(intptr_t)i);

    expect(__LINE__, 100, vec->length);
    expect(__LINE__, 0, (long)vec->data[0]);
    expect(__LINE__, 50, (long)vec->data[50]);
    expect(__LINE__, 99, (long)vec->data[99]);

    printf("OK\n");
}

