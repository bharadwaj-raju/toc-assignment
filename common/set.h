#ifndef SET_H
#define SET_H

#include <stdbool.h>

#define SET_DEFAULT_CAPACITY 10
#define KEY_LEN 128

extern int PRESENT;

typedef struct set_entry
{
    char key[KEY_LEN];
    void * data;
} set_entry_t;

typedef struct set
{
    unsigned int len;
    unsigned int capacity;
    set_entry_t * data;
} set_t;

typedef bool find_by_data_fn(void *, void *);

char * single_char_str(char c);

set_t * set_init_with_capacity(int capacity);
set_t * set_init();
set_t * set_copy(set_t * src);
void set_print(set_t * arr);
bool set_equal(set_t * s1, set_t * s2);
void set_resize(set_t * arr, int capacity);
void set_add(set_t * arr, const char * key, void * data);
void set_add_uniq(set_t * arr, const char * key, void * data);
char * set_find_by_data(set_t * arr, find_by_data_fn * fn, void * search);
void * set_find(set_t * arr, const char * key);
void set_set(set_t * arr, const char * key, void * data);
void set_clear(set_t * arr);
void set_free(set_t * arr);
void set_union_inplace(set_t * this, set_t * other);
set_t * set_intersection(set_t * s1, set_t * s2);

#endif