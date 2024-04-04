#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

stack_t * stack_init_with_capacity(int capacity)
{
    stack_t * st = malloc(sizeof(stack_t));
    st->len = 0;
    st->capacity = capacity;
    st->data = malloc(sizeof(char) * capacity);
    return st;
}

stack_t * stack_init()
{
    return stack_init_with_capacity(STACK_DEFAULT_CAPACITY);
}

stack_t * stack_copy(stack_t * other)
{
    stack_t * st = stack_init_with_capacity(other->capacity);
    st->len = other->len;
    for (size_t i = 0; i < other->len; i++) {
        st->data[i] = other->data[i];
    }
    return st;
}

void stack_print(stack_t * st, FILE * fp)
{
    printf("[ ");
    for (size_t i = 0; i < st->len; i++) {
        fprintf(fp, "%c ", st->data[i]);
    }
    printf("]");
}

void stack_resize(stack_t * st, int capacity)
{
    char * new_data = malloc(sizeof(char) * capacity);
    memcpy(new_data, st->data, sizeof(char) * st->len);
    st->capacity = capacity;
    free(st->data);
    st->data = new_data;
}

void stack_push(stack_t * st, char c)
{
    if (st->len == st->capacity) {
        stack_resize(st, st->capacity * 2);
    }
    st->data[st->len++] = c;
}

char stack_peek(stack_t * st)
{
    if (st->len == 0) {
        return '\0';
    }
    return st->data[st->len - 1];
}

char stack_pop(stack_t * st)
{
    if (st->len == 0) {
        return '\0';
    }
    char c = st->data[st->len - 1];
    st->len--;
    return c;
}

void stack_clear(stack_t * st)
{
    st->len = 0;
}

void stack_free(stack_t * st)
{
    free(st->data);
    free(st);
}