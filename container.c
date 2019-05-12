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


//
// Map
//
Map *new_map(void) {
    Map *map = malloc(sizeof(Map));
    map->keys = new_vector();
    map->values = new_vector();
    return map;
}

void map_insert(Map *map, char *key, void *val) {
    vec_push(map->keys, key);
    vec_push(map->values, val);
}

void *map_get(Map* map, char *key) {
    for (int i = map->keys->length - 1; i >= 0; i--)
        if (strcmp(map->keys->data[i], key) == 0) 
            return map->values->data[i];
    return NULL;
}

void test_map(void) {
    Map* map = new_map();
    expect(__LINE__, 0, (long)map_get(map, "foo"));

    map_insert(map, "foo", (void *)2);
    expect(__LINE__, 2, (long)map_get(map, "foo"));

    map_insert(map, "bar", (void *)4);
    expect(__LINE__, 4, (long)map_get(map, "bar"));

    map_insert(map, "foo", (void *)6);
    expect(__LINE__, 6, (long)map_get(map, "foo"));
}


