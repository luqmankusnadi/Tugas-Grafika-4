#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

void VectorInit(Vector *v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
}

int VectorTotal(Vector *v)
{
    return v->total;
}

static void VectorResize(Vector *v, int capacity)
{
    #ifdef DEBUG_ON
    printf("vector_resize: %d to %d\n", v->capacity, capacity);
    #endif

    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

void VectorAdd(Vector *v, void *item)
{
    if (v->capacity == v->total)
        VectorResize(v, v->capacity * 2);
    v->items[v->total++] = item;
}

void VectorSet(Vector *v, int index, void *item)
{
    if (index >= 0 && index < v->total)
        v->items[index] = item;
}

void *VectorGet(Vector *v, int index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return NULL;
}

void VectorDelete(Vector *v, int index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = NULL;
	int i;
    for (i = index; i < v->total - 1; i++) {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }

    v->total--;
	
    if (v->total > 0 && v->total == v->capacity / 4)
        VectorResize(v, v->capacity / 2);
       
}

void VectorFree(Vector *v)
{
    free(v->items);
}
