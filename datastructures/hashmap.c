#include "hashmap.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>

void HashMapEntry_deleter(HashMapEntry* entry) {
    deleter_f key_deleter = entry->parent->key_deleter;
    deleter_f value_deleter = entry->parent->value_deleter;
    if(key_deleter != NULL) {
        key_deleter(entry->key);
    }
    if(value_deleter != NULL) {
        value_deleter(entry->value);
    }
    free(entry);
}

HashMap* HashMap_new(deleter_f key_deleter, deleter_f value_deleter, compare_f compare, hash_f hash) {
    HashMap* result = calloc(1, sizeof(HashMap));
    HashMap_init(result, key_deleter, value_deleter, compare, hash);
    return result;
}

void HashMap_init(HashMap* hashmap, deleter_f key_deleter, deleter_f value_deleter, compare_f compare, hash_f hash) {
    hashmap->base_iter = (iterable_t) {
        .deleter = HashMapIterator_delete,
        .next = HashMapIterator_next,
        .iter = HashMap_getIter
    };
    hashmap->key_deleter = key_deleter;
    hashmap->value_deleter = value_deleter;
    hashmap->compare = compare;
    hashmap->hash = hash;
    hashmap->size = HASHMAP_GROWTH;
    hashmap->nb = 0;
    hashmap->entries = calloc(HASHMAP_GROWTH, sizeof(LinkedList*));
    for(size_t i = 0 ; i < HASHMAP_GROWTH ; i += 1) {
        hashmap->entries[i] = LinkedList_new(HashMapEntry_deleter);
    }
}

bool _compare(void* a, void* b) {
    HashMapEntry* left = (HashMapEntry*) a;
    HashMapEntry* right = (HashMapEntry*) b;
    return left->parent->compare(left->key, right->key);
}

void* _find(HashMap* hashmap, size_t hashValue, HashMapEntry* key) {
    LinkedList* list = hashmap->entries[hashValue % hashmap->size];
    HashMapEntry* entry = LinkedList_find(list, key, _compare);
    if(entry == NULL) {
        return NULL;
    }
    return entry->value;
}

void* HashMap_get(HashMap* hashmap, void* key) {
    size_t hashValue = hashmap->hash(key);
    HashMapEntry search = { .hash = hashValue, .key = key, .value = NULL, .parent = hashmap };
    return _find(hashmap, hashValue, &search);
}

void _insert(HashMap* hashmap, size_t hashValue, HashMapEntry* entry) {
    LinkedList* list = hashmap->entries[hashValue % hashmap->size];
    HashMapEntry* node = LinkedList_find(list, entry, _compare);
    if(node == NULL) {
        LinkedList_append(list, entry);
    } else {
        hashmap->key_deleter(node->key);
        hashmap->value_deleter(node->value);
        *node = *entry;
        free(entry);
    }
}

void HashMap_set(HashMap* hashmap, void* key, void* value) {
    size_t hashValue = hashmap->hash(key);
    HashMapEntry* entry = calloc(1, sizeof(HashMapEntry));
    entry->hash = hashValue;
    entry->key = key;
    entry->value = value;
    entry->parent = hashmap;
    _insert(hashmap, hashValue, entry);
}

void HashMap_clear(HashMap* hashmap) {
    for(size_t i = 0 ; i < hashmap->size ; i += 1) {
        LinkedList_delete(hashmap->entries[i]);
    }
    free(hashmap->entries);
    hashmap->entries = calloc(HASHMAP_GROWTH, sizeof(LinkedList*));
    for(size_t i = 0 ; i < HASHMAP_GROWTH ; i += 1) {
        hashmap->entries[i] = LinkedList_new(HashMapEntry_deleter);
    }
}

void HashMap_delete(HashMap* hashmap) {
    for(size_t i = 0 ; i < hashmap->size ; i += 1) {
        LinkedList_delete(hashmap->entries[i]);
    }
    free(hashmap->entries);
    free(hashmap);
}

void HashMap_print(HashMap* hashmap) {
    for(size_t i = 0 ; i < hashmap->size ; i += 1) {
        LinkedList *list = hashmap->entries[i];
        printf("[%ld]\t", i);
        LinkedList_print(list);
        printf("\n");
    }
}

iterator_t* HashMap_getIter(iterable_t* list) {
    HashMapIterator *result = calloc(1, sizeof(HashMapIterator));
    result->base._end = false;
    result->base._init = true;
    result->base.n = 0;
    result->parent = (HashMap*) list;
    HashMapIterator_next((iterator_t*) result);
    return (iterator_t*) result;
}

void HashMapIterator_next(iterator_t* iter) {
    HashMapIterator* iterator = (HashMapIterator*) iter;
    if(iter->_init) {
        iterator->current = (LinkedListIterator*) LinkedList_getIter((iterable_t*) iterator->parent->entries[iter->n]);
        iter->_init = false;
    }
    if(iter->n >= iterator->parent->size) {
        iter->_end = true;
    } else {
        while(iter->n + 1 < iterator->parent->size && iterator->current->base._end) {
            iter->n += 1;
            iterator->current = (LinkedListIterator*) LinkedList_getIter((iterable_t*) iterator->parent->entries[iter->n]);
        }
        iter->value = iterator->current->base.value;
        LinkedListIterator_next((iterator_t*) iterator->current);
    }
}

void HashMapIterator_delete(iterator_t* iter) {
    free(iter);
}