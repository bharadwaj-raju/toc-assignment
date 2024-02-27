#include <memory.h>
#include <stdbool.h>

#define DYNARRAY_DEFAULT_CAPACITY 10

typedef struct dynarray
{
    unsigned int len;
    unsigned int capacity;
    void ** data;
} dynarray_t;

typedef bool match_fn(void *);

dynarray_t * dynarray_init_with_capacity(int capacity)
{
    dynarray_t * arr = malloc(sizeof(dynarray_t));
    arr->len = 0;
    arr->capacity = capacity;
    arr->data = malloc(sizeof(void *) * arr->capacity);
    return arr;
}

dynarray_t * dynarray_init()
{
    return dynarray_init_with_capacity(DYNARRAY_DEFAULT_CAPACITY);
}

void dynarray_resize(dynarray_t * arr, int capacity)
{
    void ** new_data = malloc(sizeof(void *) * capacity);
    memcpy(new_data, arr->data, sizeof(void *) * arr->len);
    free(arr->data);
    arr->data = new_data;
    arr->capacity = capacity;
}

void dynarray_add(dynarray_t * arr, void * elem)
{
    if (arr->len == arr->capacity) {
        dynarray_resize(arr, arr->capacity * 2);
    }
    arr->data[arr->len] = elem;
    arr->len++;
}

bool dynarray_exists(dynarray_t * arr, void * elem)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (arr->data[i] == elem) {
            return true;
        }
    }
    return false;
}

void dynarray_find(dynarray_t * arr, match_fn * fn)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (fn(arr->data[i])) {
            return arr->data[i];
        }
    }
}

void dynarray_remove(dynarray_t * arr, void * elem)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (arr->data[i] == elem) {
            for (size_t j = i + 1; j < arr->len; j++) {
                arr->data[j - 1] = arr->data[j];
            }
            arr->len--;
        }
    }
}

dynarray_t * dynarray_union(dynarray_t * s1, dynarray_t * s2)
{
    dynarray_t * res = dynarray_init_with_capacity(s1->len + s2->len);
    for (size_t i = 0; i < s1->len; i++) {
        dynarray_add(res, s1->data[i]);
    }
    for (size_t i = 0; i < s2->len; i++) {
        if (!dynarray_exists(res, s2->data[i])) {
            dynarray_add(res, s2->data[i]);
        }
    }
    return res;
}

dynarray_t * dynarray_intersection(dynarray_t * s1, dynarray_t * s2)
{
    unsigned int min_len = s1->len < s2->len ? s1->len : s2->len;
    dynarray_t * min_set = s1->len < s2->len ? s1 : s2;
    dynarray_t * other_set = s1->len < s2->len ? s2 : s1;

    dynarray_t * res = dynarray_init_with_capacity(min_len);
    for (size_t i = 0; i < min_len; i++) {
        if (dynarray_exists(other_set, min_set->data[i])) {
            dynarray_add(res, min_set->data[i]);
        }
    }
}