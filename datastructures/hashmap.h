#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "common.h"
#include "iterable.h"

#define HASHMAP_GROWTH 10

/**
 * Structure representing a HashMap.
 */
typedef struct {
    iterable_t base_iter; /**< Base iterator. */
    size_t size; /**< Size of the HashMap. */
    size_t nb; /**< Number of entries in the HashMap. */
    deleter_f key_deleter; /**< Function pointer to the key deleter function. */
    deleter_f value_deleter; /**< Function pointer to the value deleter function. */
    compare_f compare; /**< Function pointer to the comparison function for keys. */
    hash_f hash; /**< Function pointer to the hash function for keys. */
    LinkedList** entries; /**< Array of linked lists representing entries in the HashMap. */
} HashMap;

/**
 * Structure representing an entry in the HashMap.
 */
typedef struct {
    size_t hash; /**< Hash value of the key. */
    void* key; /**< Pointer to the key. */
    void* value; /**< Pointer to the value associated with the key. */
    HashMap* parent; /**< Pointer to the parent HashMap. */
} HashMapEntry;

/**
 * Structure representing an iterator for the HashMap.
 */
typedef struct {
    iterator_t base; /**< Base iterator. */
    HashMap* parent; /**< Pointer to the parent HashMap. */
    LinkedListIterator* current; /**< Pointer to the current linked list iterator. */
} HashMapIterator;

/**
 * Creates a new HashMap object.
 * @param key_deleter A function pointer to the key deleter function.
 * @param value_deleter A function pointer to the value deleter function.
 * @param compare A function pointer to the comparison function for keys.
 * @param hash A function pointer to the hash function for keys.
 * @return A pointer to the newly created HashMap object.
 */
HashMap* HashMap_new(deleter_f key_deleter, deleter_f value_deleter, compare_f compare, hash_f hash);

/**
 * Initializes a HashMap object.
 * @param hashmap A pointer to the HashMap object to be initialized.
 * @param key_deleter A function pointer to the key deleter function.
 * @param value_deleter A function pointer to the value deleter function.
 * @param compare A function pointer to the comparison function for keys.
 * @param hash A function pointer to the hash function for keys.
 */
void HashMap_init(HashMap* hashmap, deleter_f key_deleter, deleter_f value_deleter, compare_f compare, hash_f hash);

/**
 * Sets a key-value pair in the HashMap.
 * @param hashmap A pointer to the HashMap object.
 * @param key A pointer to the key.
 * @param value A pointer to the value.
 */
void HashMap_set(HashMap* hashmap, void* key, void* value);

/**
 * Gets the value associated with a key in the HashMap.
 * @param hashmap A pointer to the HashMap object.
 * @param key A pointer to the key.
 * @return A pointer to the value associated with the key, or NULL if not found.
 */
void* HashMap_get(HashMap* hashmap, void* key);

/**
 * Deletes the HashMap object and frees associated resources.
 * @param hashmap A pointer to the HashMap object to be deleted.
 */
void HashMap_delete(HashMap* hashmap);

/**
 * Clears all entries from the HashMap.
 * @param hashmap A pointer to the HashMap object.
 */
void HashMap_clear(HashMap* hashmap);

/**
 * Prints the contents of the HashMap.
 * @param hashmap A pointer to the HashMap object to be printed.
 */
void HashMap_print(HashMap* hashmap);

/**
 * Deleter function for HashMapEntry objects.
 * @param entry A pointer to the HashMapEntry object to be deleted.
 */
void HashMapEntry_deleter(HashMapEntry* entry);

iterator_t* HashMap_getIter(iterable_t* list);
void HashMapIterator_next(iterator_t* iter);
void HashMapIterator_delete(iterator_t* iter);


#endif