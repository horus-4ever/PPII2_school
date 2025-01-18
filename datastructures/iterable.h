#ifndef ITERABLE_H
#define ITERABLE_H

#include <stddef.h>
#include <stdbool.h>

#define FOREACH(elem, list, block) for(elem = ((iterable_t*)list)->iter((iterable_t*)list) ; !elem->_end ; ((iterable_t*)list)->next(elem)) block;  ((iterable_t*)list)->deleter(elem);

typedef struct {
    size_t n;
    void* value;
    bool _end;
    bool _init;
} iterator_t;

typedef struct _iterable_t {
    iterator_t* (*iter)(struct _iterable_t* iterable);
    void (*next)(iterator_t* iter);
    void (*deleter)(iterator_t* iter);
} iterable_t;

#endif