// Vector
typedef struct {
    void **data;
    int capacity;
    int length;
} Vector;

Vector *new_vector(void);
void vec_push(Vector *v, void *element);
void test_vector(void);

// Map
typedef struct {
    Vector *keys;
    Vector *values;
} Map;

Map *new_map(void);
void map_insert(Map *map, char *key, void *val);
void *map_get(Map* map, char *key);
void test_map(void);


