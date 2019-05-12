// Vector
typedef struct {
    void **data;
    int capacity;
    int length;
} Vector;

// container.c
Vector *new_vector(void);
void vec_push(Vector *v, void *element);
void test_vector(void);


