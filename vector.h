#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_INIT_CAPACITY 4

typedef struct Vector {
    void **items;
    int capacity;
    int total;
} Vector;

void VectorInit(Vector *);
int VectorTotal(Vector *);
static void VectorResize(Vector *, int);
void VectorAdd(Vector *, void *);
void VectorSet(Vector *, int, void *);
void *VectorGet(Vector *, int);
void VectorDelete(Vector *, int);
void VectorFree(Vector *);

#endif
