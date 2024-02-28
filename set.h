#ifndef SET_H
#define SET_H

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

set_t * set_init_with_capacity(int capacity);
set_t * set_init();
void set_print(set_t * arr);
void set_resize(set_t * arr, int capacity);
void set_add(set_t * arr, const char * key, void * data);
void * set_find(set_t * arr, const char * key);
void set_remove(set_t * arr, const char * key);
void set_clear(set_t * arr);
set_t * set_union(set_t * s1, set_t * s2);
void set_union_inplace(set_t * this, set_t * other);
set_t * set_intersection(set_t * s1, set_t * s2);

#endif