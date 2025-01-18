#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>

LinkedList* LinkedList_new(deleter_f deleter) {
    LinkedList* result = malloc(sizeof(LinkedList));
    LinkedList_init(result, deleter);
    return result;
}

void LinkedList_init(LinkedList* list, deleter_f deleter) {
    list->base_iter = (iterable_t) {
        .deleter = LinkedListIterator_delete,
        .next = LinkedListIterator_next,
        .iter = LinkedList_getIter
    };
    list->deleter = deleter;
    list->head = NULL;
    list->length = 0;
}

void LinkedList_append_first(LinkedList* list, void* value) {
    LinkedListNode* ptr = list->head;
    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    node->value = value;
    node->next = ptr;
    list->head = node;
    list->length += 1;
}

void LinkedList_append(LinkedList* list, void* value) {
    LinkedListNode* ptr = list->head;
    if(ptr == NULL) {
        LinkedList_append_first(list, value);
    } else {
        LinkedListNode* node = malloc(sizeof(LinkedListNode));
        node->value = value;
        node->next = NULL;
        list->length += 1;
        while(ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = node;
    }
}

void* LinkedList_get(LinkedList* list, size_t n) {
    if(n >= list->length) {
        return NULL;
    }
    LinkedListNode* ptr = list->head;
    while(n != 0) {
        ptr = ptr->next;
    }
    return ptr->value;
}

void* LinkedList_find(LinkedList* list, void* value, compare_f compare) {
    LinkedListNode* ptr = list->head;
    while(ptr != NULL) {
        if(compare(value, ptr->value)) {
            return ptr->value;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void LinkedList_pop(LinkedList* list) {
    LinkedListNode* ptr = list->head;
    list->length -= 1;
    if(ptr->next == NULL) {
        LinkedListNode_delete(ptr, list->deleter);
        list->head = NULL;
        return;
    }
    while(ptr->next->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = NULL;
}

void LinkedList_clear(LinkedList* list) {
    LinkedListNode* ptr = list->head;
    while(ptr != NULL) {
        LinkedListNode* temp = ptr->next;
        LinkedListNode_delete(ptr, list->deleter);
        ptr = temp;
    }
    list->length = 0;
}

void* LinkedList_head(LinkedList* list) {
    return list->head->value;
}

size_t LinkedList_length(LinkedList* list) {
    return list->length;
}

void LinkedList_delete(LinkedList* list) {
    LinkedList_clear(list);
    free(list);
}

void LinkedListNode_delete(LinkedListNode* node, deleter_f deleter) {
    deleter(node->value);
    free(node);
}

void LinkedList_print(LinkedList* list) {
    LinkedListNode* ptr = list->head;
    while(ptr != NULL) {
        printf("%p -> ", ptr->value);
        ptr = ptr->next;
    }
    printf("NULL");
}

iterator_t* LinkedList_getIter(iterable_t* list) {
    LinkedListIterator *result = calloc(1, sizeof(LinkedListIterator));
    result->base._end = false;
    result->base._init = true;
    result->base.n = 0;
    result->next = ((LinkedList*) list)->head;
    LinkedListIterator_next((iterator_t*) result);
    return (iterator_t*) result;
}

void LinkedListIterator_next(iterator_t* iter) {
    LinkedListIterator* iterator = (LinkedListIterator*) iter;
    size_t next_n = iter->n + 1;
    if(iter->_init) {
        next_n = 0;
        iter->_init = false;
    }
    if(iterator->next == NULL) {
        iter->_end = true;
    } else {
        iter->value = iterator->next->value;
        iterator->next = iterator->next->next;
        iter->n = next_n;
    }
}

void LinkedListIterator_delete(iterator_t* iter) {
    free(iter);
}