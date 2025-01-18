#ifndef TN_OBJECT_H
#define TN_OBJECT_H

#include "../utf8strings/utf8strings.h"
#include "../datastructures/array.h"
#include "../perso_SDL.h"
#include "../quest/quest.h"
#include <stdbool.h>

struct Tn_Object;

typedef struct Tn_Object*(*__getattr__f)(struct Tn_Object*, const char*);
typedef void(*__setattr__f)(struct Tn_Object*, const char*, struct Tn_Object*);
typedef struct Tn_Object*(*__eq__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__neq__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__ge__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__le__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__gt__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__lt__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__add__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__sub__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__mul__f)(struct Tn_Object*, struct Tn_Object*);
typedef struct Tn_Object*(*__call__f)(struct Tn_Object*, struct Tn_Object*, ArrayList*);
typedef struct Tn_Object*(*__repr__f)(struct Tn_Object*);

/**
 * Enumeration of Tn object types.
 */
typedef enum {
    STRING_OBJ, /**< String object type. */
    NUMBER_OBJ, /**< Number object type. */
    BOOL_OBJ, /**< Boolean object type. */
    PLAYER_OBJ, /**< Player object type. */
    QUEST_OBJ, /**< Quest object type. */
    CHARACTER_OBJ, /**< Character object type. */
    FUNCTION_OBJ /**< Function object type. */
} Tn_ObjectType;

/**
 * Structure representing a Tn object.
 */
typedef struct Tn_Object {
    Tn_ObjectType type; /**< Type of the Tn object. */
    union {
        UTF8_String* string; /**< Pointer to the string value if the object is of type STRING_OBJ. */
        int64_t number; /**< Numeric value if the object is of type NUMBER_OBJ. */
        bool boolean; /**< Boolean value if the object is of type BOOL_OBJ. */
        Perso_SDL* player; /**< Pointer to the Perso_SDL representing the player if the object is of type PLAYER_OBJ. */
        Character* character; /**< Pointer to the Character object if the object is of type CHARACTER_OBJ. */
        Quest* quest; /**< Pointer to the Quest object if the object is of type QUEST_OBJ. */
        struct Tn_Object*(*function)(); /**< Pointer to the function if the object is of type FUNCTION_OBJ. */
    };
    __getattr__f getattr; /**< Pointer to the getattr function. */
    __setattr__f setattr; /**< Pointer to the setattr function. */
    __eq__f equal; /**< Pointer to the equal function. */
    __neq__f nequal; /**< Pointer to the nequal function. */
    __ge__f gequal; /**< Pointer to the gequal function. */
    __le__f lequal; /**< Pointer to the lequal function. */
    __gt__f gthan; /**< Pointer to the gthan function. */
    __lt__f lthan; /**< Pointer to the lthan function. */
    __add__f add; /**< Pointer to the add function. */
    __sub__f sub; /**< Pointer to the sub function. */
    __mul__f mul; /**< Pointer to the mul function. */
    __call__f call; /**< Pointer to the call function. */
    __repr__f repr; /**< Pointer to the repr function. */
    int refcount; /**< Reference count for garbage collection. */
} Tn_Object;

/**
 * Gets a string representation of a Tn object type.
 * @param type The Tn object type.
 * @return A string representation of the Tn object type.
 */
const char* Tn_ObjectType_repr(Tn_ObjectType type);

/**
 * Initializes a Tn object.
 * @param object A pointer to the Tn object to be initialized.
 */
void Tn_Object_init(Tn_Object* object);

/**
 * Returns a string representation of the Tn object.
 * @param self A pointer to the Tn object.
 * @return A pointer to the string representation of the Tn object.
 */
Tn_Object* Tn_Object_repr(Tn_Object* self);

/**
 * Calls the function associated with a Tn object.
 * @param self A pointer to the Tn object.
 * @param object A pointer to the Tn object on which the function is called.
 * @param args A pointer to the ArrayList containing function arguments.
 * @return A pointer to the result of the function call.
 */
Tn_Object* Tn_Object_Function_call(Tn_Object* self, Tn_Object* object, ArrayList* args);

/**
 * Gets an attribute of the Tn object.
 * @param self A pointer to the Tn object.
 * @param name The name of the attribute.
 * @return A pointer to the attribute value.
 */
Tn_Object* Tn_Object_getattr(Tn_Object* self, const char* name);

/**
 * Sets an attribute of the Tn object.
 * @param self A pointer to the Tn object.
 * @param name The name of the attribute.
 * @param object A pointer to the value of the attribute.
 */
void Tn_Object_setattr(Tn_Object* self, const char* name, Tn_Object* object);

/**
 * Checks if two Tn objects are equal.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the comparison.
 */
Tn_Object* Tn_Object_equal(Tn_Object* self, Tn_Object* object);

/**
 * Checks if two Tn objects are not equal.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the comparison.
 */
Tn_Object* Tn_Object_nequal(Tn_Object* self, Tn_Object* object);

/**
 * Checks if the first Tn object is greater than or equal to the second.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the comparison.
 */
Tn_Object* Tn_Object_gequal(Tn_Object* self, Tn_Object* object);

/**
 * Checks if the first Tn object is less than or equal to the second.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the comparison.
 */
Tn_Object* Tn_Object_lequal(Tn_Object* self, Tn_Object* object);

/**
 * Checks if the first Tn object is greater than the second.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the comparison.
 */
Tn_Object* Tn_Object_gthan(Tn_Object* self, Tn_Object* object);

/**
 * Checks if the first Tn object is less than the second.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the comparison.
 */
Tn_Object* Tn_Object_lthan(Tn_Object* self, Tn_Object* object);

/**
 * Adds two Tn objects.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the addition.
 */
Tn_Object* Tn_Object_add(Tn_Object* self, Tn_Object* object);

/**
 * Subtracts two Tn objects.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the subtraction.
 */
Tn_Object* Tn_Object_sub(Tn_Object* self, Tn_Object* object);

/**
 * Multiplies two Tn objects.
 * @param self A pointer to the first Tn object.
 * @param object A pointer to the second Tn object.
 * @return A pointer to the result of the multiplication.
 */
Tn_Object* Tn_Object_mul(Tn_Object* self, Tn_Object* object);

/**
 * Increases the reference count of a Tn object.
 * @param object A pointer to the Tn object.
 */
void Tn_Object_INCREF(Tn_Object* object);

/**
 * Decreases the reference count of a Tn object and deletes it if the count reaches zero.
 * @param object A pointer to the Tn object.
 */
void Tn_Object_DECREF(Tn_Object* object);

/**
 * Deletes a Tn object and frees associated resources.
 * @param object A pointer to the Tn object to be deleted.
 */
void Tn_Object_delete(Tn_Object* object);

/**
 * Raises an attribute error for the given object and attribute name.
 * @param object A pointer to the Tn object.
 * @param name The name of the attribute.
 */
void Tn_Object_ATTRIBUTE_ERROR(Tn_Object* object, const char* name);

#endif