#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include "iterable.h"
#include "common.h"

#define ARRAY_GROWTH 20

/**
 * Structure representing an ArrayList.
 */
typedef struct {
    iterable_t base_iter; /**< Base iterator. */
    size_t length; /**< Current length of the ArrayList. */
    size_t reserved; /**< Reserved memory for the ArrayList. */
    deleter_f deleter; /**< Function pointer to the deleter function for freeing elements. */
    void** data; /**< Pointer to the data array of the ArrayList. */
} ArrayList;

/**
 * Structure representing an ArrayList iterator.
 */
typedef struct {
    iterator_t base; /**< Base iterator. */
    ArrayList* parent; /**< Pointer to the parent ArrayList. */
} ArrayListIterator;

/**
 * Creates a new ArrayList object.
 * @param deleter A function pointer to the deleter function for freeing elements.
 * @return A pointer to the newly created ArrayList object.
 */
ArrayList* ArrayList_new(deleter_f deleter);

/**
 * Initializes an ArrayList object.
 * @param list A pointer to the ArrayList object to be initialized.
 * @param deleter A function pointer to the deleter function for freeing elements.
 */
void ArrayList_init(ArrayList* list, deleter_f deleter);

/**
 * Appends a value to the end of the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @param value A pointer to the value to be appended.
 */
void ArrayList_append(ArrayList* list, void* value);

/**
 * Appends a value to the beginning of the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @param value A pointer to the value to be appended.
 */
void ArrayList_append_first(ArrayList* list, void* value);

/**
 * Removes and returns the last value from the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @return A pointer to the removed value.
 */
void* ArrayList_pop(ArrayList* list);

/**
 * Clears the ArrayList, removing all elements.
 * @param list A pointer to the ArrayList object.
 */
void ArrayList_clear(ArrayList* list);

/**
 * Deletes the ArrayList object and frees associated resources.
 * @param list A pointer to the ArrayList object to be deleted.
 */
void ArrayList_delete(ArrayList* list);

/**
 * Removes a specific object from the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @param object A pointer to the object to be removed.
 */
void ArrayList_remove(ArrayList* list, void* object);

/**
 * Removes the element at the specified index from the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @param index The index of the element to be removed.
 */
void ArrayList_removeAt(ArrayList* list, size_t index);

/**
 * Returns the first element of the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @return A pointer to the first element.
 */
void* ArrayList_head(ArrayList* list);

/**
 * Returns the element at the specified index in the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @param n The index of the element to retrieve.
 * @return A pointer to the element at the specified index.
 */
void* ArrayList_get(ArrayList* list, size_t n);

/**
 * Finds the first occurrence of a value in the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @param value A pointer to the value to find.
 * @param compare_f A function pointer to the comparison function.
 * @return A pointer to the found value, or NULL if not found.
 */
void* ArrayList_find(ArrayList* list, void* value, compare_f);

/**
 * Prints the contents of the ArrayList.
 * @param list A pointer to the ArrayList object to be printed.
 */
void ArrayList_print(ArrayList* list);

/**
 * Returns the current length of the ArrayList.
 * @param list A pointer to the ArrayList object.
 * @return The current length of the ArrayList.
 */
size_t ArrayList_length(ArrayList* list);

iterator_t* ArrayList_getIter(iterable_t* list);
void ArrayListIterator_next(iterator_t* iter);
void ArrayListIterator_delete(iterator_t* iter);

#endif