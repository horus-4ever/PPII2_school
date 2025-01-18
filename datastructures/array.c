#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "array.h"

void _realloc(ArrayList* list, size_t length) {
    size_t needed = ARRAY_GROWTH * (length / ARRAY_GROWTH + 1);
    void** newData = calloc(needed, sizeof(void*));
    if(list->data != NULL) {
        memcpy(newData, list->data, list->length * sizeof(void*));
        free(list->data);
    }
    list->data = newData;
    list->reserved = needed;
}

ArrayList* ArrayList_new(deleter_f deleter) {
    ArrayList* result = calloc(1, sizeof(ArrayList));
    ArrayList_init(result, deleter);
    return result;
}

void ArrayList_init(ArrayList* list, deleter_f deleter) {
    list->base_iter = (iterable_t) {
        .deleter = ArrayListIterator_delete,
        .next = ArrayListIterator_next,
        .iter = ArrayList_getIter
    };
    list->data = NULL;
    list->deleter = deleter;
    list->length = 0;
    list->reserved = 0;
    _realloc(list, 0);
}

void ArrayList_append(ArrayList* list, void* value) {
    if(list->length == list->reserved) {
        _realloc(list, list->length + 1);
    }
    list->data[list->length] = value;
    list->length += 1;
}

void ArrayList_append_first(ArrayList* list, void* value) {
    if(list->length == list->reserved) {
        _realloc(list, list->length + 1);
    }
    memmove(list->data + 1, list->data, list->length * sizeof(void*));
    list->data[0] = value;
    list->length += 1;
}

void* ArrayList_pop(ArrayList* list) {
    void* value = ArrayList_get(list, list->length - 1);
    list->length -= 1;
    return value;
}

void ArrayList_clear(ArrayList* list) {
    for(size_t i = 0 ; i < list->length ; i += 1) {
        if(list->deleter != NULL) {
            list->deleter(list->data[i]);
        }
    }
    list->length = 0;
    list->reserved = 0;
    _realloc(list, 0);
}

void ArrayList_delete(ArrayList* list) {
    ArrayList_clear(list);
    free(list->data);
    free(list);
}

void* ArrayList_head(ArrayList* list) {
    return list->data[0];
}

void* ArrayList_get(ArrayList* list, size_t n) {
    if(n >= list->length) {
        return NULL;
    }
    return list->data[n];
}

void ArrayList_removeAt(ArrayList* list, size_t index) {
    if(list->deleter != NULL) {
        list->deleter(list->data[index]);
    }
    for(size_t i = index + 1 ; i < list->length ; i += 1) {
        list->data[i - 1] = list->data[i];
    }
    list->length -= 1;
    _realloc(list, list->length);
}

void ArrayList_remove(ArrayList* list, void* object) {
    for(size_t i = 0 ; i < list->length ; i += 1) {
        if(object == list->data[i]) {
            ArrayList_removeAt(list, i);
            return;
        }
    }
}

void* ArrayList_find(ArrayList* list, void* value, compare_f compare) {
    for(size_t i = 0 ; i < list->length ; i += 1) {
        if(compare != NULL && compare(list->data[i], value)) {
            return list->data[i];
        } else if(compare == NULL && list->data[i] == value) {
            return list->data[i];
        }
    }
    return NULL;
}

size_t ArrayList_length(ArrayList* list) {
    return list->length;
}

void ArrayList_print(ArrayList* list) {
    printf("[ ");
    for(size_t i = 0 ; i < list->length ; i += 1) {
        printf("%p ", list->data[i]);
    }
    printf("]\n");
}

iterator_t* ArrayList_getIter(iterable_t* list) {
    ArrayListIterator *result = calloc(1, sizeof(ArrayListIterator));
    result->base._end = false;
    result->base._init = true;
    result->parent = (ArrayList*) list;
    ArrayListIterator_next((iterator_t*) result);
    return (iterator_t*) result;
}

void ArrayListIterator_next(iterator_t* iter) {
    ArrayListIterator* iterator = (ArrayListIterator*) iter;
    size_t next = iter->n + 1;
    if(iter->_init) {
        next = 0;
        iter->_init = false;
    }
    if(next >= iterator->parent->length) {
        iter->_end = true;
    } else {
        iter->value = iterator->parent->data[next];
        iter->n = next;
    }
}

void ArrayListIterator_delete(iterator_t* iter) {
    free(iter);
}