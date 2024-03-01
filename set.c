#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

int PRESENT = 123;

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

set_t * set_copy(set_t * src)
{
    set_t * copy = set_init_with_capacity(src->len);
    copy->len = src->len;
    memcpy(copy->data, src->data, sizeof(set_entry_t) * src->len);
    return copy;
}

void set_print(set_t * arr)
{
    printf("{ ");
    for (size_t i = 0; i < arr->len; i++) {
        printf("%s ", arr->data[i].key);
    }
    printf("}\n");
}

bool set_equal(set_t * s1, set_t * s2)
{
    if (s1->len != s2->len) {
        return false;
    }
    for (size_t i = 0; i < s1->len; i++) {
        if (set_find(s2, s1->data[i].key) == NULL) {
            return false;
        }
    }
    return true;
}

void set_resize(set_t * arr, int capacity)
{
    set_entry_t * new_data = malloc(sizeof(set_entry_t) * capacity);
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

void set_add_uniq(set_t * arr, const char * key, void * data)
{
    if (set_find(arr, key) == NULL) {
        set_add(arr, key, data);
    }
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

char * set_find_by_data(set_t * arr, find_by_data_fn * fn, void * search)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (fn(search, arr->data[i].data)) {
            return arr->data[i].key;
        }
    }
    return NULL;
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
        set_add_uniq(res, s2->data[i].key, s2->data[i].data);
    }
    return res;
}

void set_union_inplace(set_t * this, set_t * other)
{
    for (size_t i = 0; i < other->len; i++) {
        set_add_uniq(this, other->data[i].key, other->data[i].data);
    }
}

set_t * set_intersection(set_t * s1, set_t * s2)
{
    unsigned int min_len = s1->len < s2->len ? s1->len : s2->len;
    set_t * min_set = s1->len < s2->len ? s1 : s2;
    set_t * other_set = s1->len < s2->len ? s2 : s1;

    set_t * res = set_init_with_capacity(min_len);
    for (size_t i = 0; i < min_len; i++) {
        if (set_find(other_set, min_set->data[i].key) != NULL) {
            set_add_uniq(res, min_set->data[i].key, min_set->data[i].data);
        }
    }
    return res;
}