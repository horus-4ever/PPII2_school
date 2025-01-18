#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "common.h"
#include "iterable.h"

/**
 * Structure representing a node in a linked list.
 */
typedef struct _LinkedListNode {
    void* value; /**< Pointer to the value stored in the node. */
    struct _LinkedListNode* next; /**< Pointer to the next node in the linked list. */
} LinkedListNode;

/**
 * Structure representing a linked list.
 */
typedef struct _LinkedList {
    iterable_t base_iter; /**< Base iterator. */
    deleter_f deleter; /**< Function pointer to the deleter function for freeing elements. */
    LinkedListNode* head; /**< Pointer to the head node of the linked list. */
    size_t length; /**< Length of the linked list. */
} LinkedList;

/**
 * Structure representing an iterator for a linked list.
 */
typedef struct {
    iterator_t base; /**< Base iterator. */
    LinkedListNode* next; /**< Pointer to the next node in the linked list. */
} LinkedListIterator;

/**
 * Creates a new LinkedList object.
 * @param deleter A function pointer to the deleter function for freeing elements.
 * @return A pointer to the newly created LinkedList object.
 */
LinkedList* LinkedList_new(deleter_f deleter);

/**
 * Initializes a LinkedList object.
 * @param list A pointer to the LinkedList object to be initialized.
 * @param deleter A function pointer to the deleter function for freeing elements.
 */
void LinkedList_init(LinkedList* list, deleter_f deleter);

/**
 * Appends a value to the end of the linked list.
 * @param list A pointer to the LinkedList object.
 * @param value A pointer to the value to be appended.
 */
void LinkedList_append(LinkedList* list, void* value);

/**
 * Appends a value to the beginning of the linked list.
 * @param list A pointer to the LinkedList object.
 * @param value A pointer to the value to be appended.
 */
void LinkedList_append_first(LinkedList* list, void* value);

/**
 * Removes the last element from the linked list.
 * @param list A pointer to the LinkedList object.
 */
void LinkedList_pop(LinkedList* list);

/**
 * Clears the linked list, removing all elements.
 * @param list A pointer to the LinkedList object.
 */
void LinkedList_clear(LinkedList* list);

/**
 * Deletes the LinkedList object and frees associated resources.
 * @param list A pointer to the LinkedList object to be deleted.
 */
void LinkedList_delete(LinkedList* list);

/**
 * Returns the value of the head node of the linked list.
 * @param list A pointer to the LinkedList object.
 * @return A pointer to the value of the head node.
 */
void* LinkedList_head(LinkedList* list);

/**
 * Returns the value at the specified index in the linked list.
 * @param list A pointer to the LinkedList object.
 * @param n The index of the value to retrieve.
 * @return A pointer to the value at the specified index.
 */
void* LinkedList_get(LinkedList* list, size_t n);

/**
 * Finds the first occurrence of a value in the linked list.
 * @param list A pointer to the LinkedList object.
 * @param value A pointer to the value to find.
 * @param compare_f A function pointer to the comparison function.
 * @return A pointer to the found value, or NULL if not found.
 */
void* LinkedList_find(LinkedList* list, void* value, compare_f);

/**
 * Returns the length of the linked list.
 * @param list A pointer to the LinkedList object.
 * @return The length of the linked list.
 */
size_t LinkedList_length(LinkedList* list);

/**
 * Prints the contents of the linked list.
 * @param list A pointer to the LinkedList object to be printed.
 */
void LinkedList_print(LinkedList* list);

/**
 * Deletes a LinkedListNode object and frees associated resources.
 * @param node A pointer to the LinkedListNode object to be deleted.
 * @param deleter A function pointer to the deleter function for freeing elements.
 */
void LinkedListNode_delete(LinkedListNode* node, deleter_f deleter);

iterator_t* LinkedList_getIter(iterable_t* list);
void LinkedListIterator_next(iterator_t* iter);
void LinkedListIterator_delete(iterator_t* iter);

#endif