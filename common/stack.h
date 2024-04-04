#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdio.h>

#define STACK_DEFAULT_CAPACITY 10

typedef struct stack
{
    unsigned int len;
    unsigned int capacity;
    char * data;
} stack_t;

stack_t * stack_init_with_capacity(int capacity);
stack_t * stack_init();
stack_t * stack_copy(stack_t * other);
void stack_print(stack_t * st, FILE * fp);
void stack_resize(stack_t * st, int capacity);
void stack_push(stack_t * st, char c);
char stack_peek(stack_t * st);
char stack_pop(stack_t * st);
void stack_clear(stack_t * st);
void stack_free(stack_t * st);

#endif