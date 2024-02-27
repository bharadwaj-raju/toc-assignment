#include <memory.h>
#include <stdbool.h>

#include "set.h"

set_t * set_init_with_capacity(int capacity)
{
    set_t * arr = malloc(sizeof(set_t));
    arr->len = 0;
    arr->capacity = capacity;
    arr->data = malloc(sizeof(set_entry_t) * arr->capacity);
    return arr;
}

set_t * set_init()
{
    return set_init_with_capacity(SET_DEFAULT_CAPACITY);
}

void set_resize(set_t * arr, int capacity)
{
    void ** new_data = malloc(sizeof(set_entry_t) * capacity);
    memcpy(new_data, arr->data, sizeof(set_entry_t) * arr->len);
    free(arr->data);
    arr->data = new_data;
    arr->capacity = capacity;
}

void set_add(set_t * arr, const char * key, void * data)
{
    if (arr->len == arr->capacity) {
        set_resize(arr, arr->capacity * 2);
    }

    set_entry_t entry;
    strncpy(entry.key, key, KEY_LEN);
    entry.data = data;

    arr->data[arr->len] = entry;
    arr->len++;
}

void * set_find(set_t * arr, const char * key)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (strncmp(arr->data[i].key, key, KEY_LEN) == 0) {
            return arr->data[i].data;
        }
    }
    return NULL;
}

void set_remove(set_t * arr, const char * key)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (strncmp(arr->data[i].key, key, KEY_LEN) == 0) {
            for (size_t j = i + 1; j < arr->len; j++) {
                arr->data[j - 1] = arr->data[j];
            }
            arr->len--;
        }
    }
}

void set_clear(set_t * arr)
{
    arr->len = 0;
    free(arr->data);
    arr->data = malloc(sizeof(set_entry_t) * arr->capacity);
}

set_t * set_union(set_t * s1, set_t * s2)
{
    set_t * res = set_init_with_capacity(s1->len + s2->len);
    for (size_t i = 0; i < s1->len; i++) {
        set_add(res, s1->data[i].key, s1->data[i].data);
    }
    for (size_t i = 0; i < s2->len; i++) {
        if (set_find(res, s2->data[i].key) == NULL) {
            set_add(res, s2->data[i].key, s2->data[i].data);
        }
    }
    return res;
}

void * set_union_inplace(set_t * this, set_t * other)
{
    for (size_t i = 0; i < other->len; i++) {
        if (set_find(this, other->data[i].key) == NULL) {
            set_add(this, other->data[i].key, other->data[i].data);
        }
    }
}

set_t * set_intersection(set_t * s1, set_t * s2)
{
    unsigned int min_len = s1->len < s2->len ? s1->len : s2->len;
    set_t * min_set = s1->len < s2->len ? s1 : s2;
    set_t * other_set = s1->len < s2->len ? s2 : s1;

    set_t * res = set_init_with_capacity(min_len);
    for (size_t i = 0; i < min_len; i++) {
        if (set_find(other_set, min_set->data[i].key) != NULL && set_find(res, min_set->data[i].key) == NULL) {
            set_add(res, min_set->data[i].key, min_set->data[i].data);
        }
    }
    return res;
}