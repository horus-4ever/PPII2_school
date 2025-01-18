#ifndef UTF8_STRINGS_H
#define UTF8_STRINGS_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Represents a UTF-8 string.
 */
typedef struct _UTF8_string {
  /** Pointer to the string data (owned and read-only). */
  uint8_t* data;
  /** Size of the string data in bytes. */
  size_t size;
  /** Length of the string in UTF-8 code points. */
  size_t length;
} UTF8_String;

/**
 * Represents a slice of a UTF-8 string.
 */
typedef struct {
  /** Pointer to the parent UTF-8 string. */
  const UTF8_String* parent;
  /** Index of the first character in the slice (within the parent string). */
  size_t begin;
  /** Index of the character after the last character in the slice (within the parent string). */
  size_t end;
  /** Real beginning of the slice considering UTF-8 character boundaries (within the parent string). */
  size_t real_begin;
  /** Real end of the slice considering UTF-8 character boundaries (within the parent string). */
  size_t real_end;
} UTF8_Slice;

/**
 * Creates a new UTF-8 string from a null-terminated C string.
 * @param string The null-terminated C string.
 * @return A new UTF-8 string object.
 */
UTF8_String* UTF8_from(const char* string);

/**
 * Creates an empty UTF-8 string object.
 * @return A new empty UTF-8 string object.
 */
UTF8_String* UTF8_empty();

/**
 * Creates a copy of a UTF-8 string.
 * @param string The UTF-8 string to copy.
 * @return A new UTF-8 string object (copy of the input string).
 */
UTF8_String* UTF8_copy(UTF8_String const* string);

bool UTF8_equal(UTF8_String const* s1, UTF8_String const* s2);
size_t UTF8_hash(UTF8_String const* string);

/**
 * Gets the size of a UTF-8 string in bytes.
 * @param string The UTF-8 string.
 * @return The size of the string data in bytes.
 */
size_t UTF8_size(UTF8_String const* string);

/**
 * Gets the length of a UTF-8 string in UTF-8 code points.
 * @param string The UTF-8 string.
 * @return The length of the string in UTF-8 code points.
 */
size_t UTF8_length(UTF8_String const* string);

/**
 * Appends a string to the end of a UTF-8 string object.
 * @param string The UTF-8 string object to modify.
 * @param str The null-terminated UTF-8 string to append.
 */
void UTF8_append(UTF8_String* string, const char* str);

/**
 * Pops (removes and returns) the last character from a UTF-8 string object.
 * @param string The UTF-8 string object to modify.
 * @return The last character of the string, or an empty string if the string was empty.
 */
UTF8_String* UTF8_pop(UTF8_String* string);

/**
 * Internal function for calculating the UTF-8 length of a string (not intended for public use).
 * @param string The null-terminated C string.
 * @return The length of the string in UTF-8 code points.
 */
size_t UTF8_utf8length(const char* string);

/**
 * Creates a slice of a UTF-8 string.
 * @param string The parent UTF-8 string.
 * @param begin Index of the first character in the slice (within the parent string).
 * @param end Index of the character after the last character in the slice (within the parent string).
 * @return A new UTF-8 slice object.
 */
UTF8_Slice* UTF8_slice(UTF8_String const* string, size_t begin, size_t end);

/**
 * Creates an empty UTF-8 slice.
 * @return An empty UTF-8 slice object.
 */
UTF8_Slice* UTF8_emptySlice();

/**
 * Checks if a UTF-8 slice is empty.
 * @param slice The UTF-8 slice to check.
 * @return True if the slice is empty, false otherwise.
 */
bool UTF8_isEmptySlice(const UTF8_Slice* slice);

/**
 * Converts a UTF-8 slice to a new UTF-8 string.
 * @param slice The UTF-8 slice.
 * @return A new UTF-8 string object representing the slice.
 */
UTF8_String* UTF8_toString(const UTF8_Slice* slice);

/**
 * Gets a pointer to the data of a UTF-8 slice.
 * @param slice The UTF-8 slice.
 * @return A pointer to the slice data.
 */
const char* UTF8_slicePtr(const UTF8_Slice* slice);

/**
 * Gets the length of a UTF-8 slice in UTF-8 code points.
 * @param slice The UTF-8 slice.
 * @return The length of the slice in UTF-8 code points.
 */
size_t UTF8_sliceLength(const UTF8_Slice* slice);

/**
 * Gets the size of a UTF-8 slice in bytes.
 * @param slice The UTF-8 slice.
 * @return The size of the slice data in bytes.
 */
size_t UTF8_sliceSize(const UTF8_Slice* slice);

/**
 * Deallocates a UTF-8 string object.
 * @param string The UTF-8 string to deallocate.
 * @note This function _must_ be used to deallocate memory for UTF-8 strings created using `UTF8_from` or `UTF8_copy`. Do not use `free` directly on these objects.
 */
void UTF8_delete(UTF8_String* string);

/**
 * Gets a null-terminated C string representation of a UTF-8 string.
 * @param string The UTF-8 string.
 * @return A null-terminated C string representation of the UTF-8 string.
 * @note The returned string is owned by the callee and should not be modified or freed.
 */
char* UTF8_str(const UTF8_String* string);

/**
 * Appends a character to a UTF-8 string.
 * @param string The UTF-8 string.
 * @param c The character to append.
 */
void UTF8_appendChar(UTF8_String* string, char c);

/**
 * Creates a UTF-8 string from the content of a file.
 * @param filename The name of the file.
 * @return A pointer to the newly created UTF-8 string.
 */
UTF8_String* UTF8_fromFile(const char* filename);

/**
 * Converts a UTF-8 string to an integer.
 * @param string The UTF-8 string.
 * @return The integer value.
 */
int UTF8_toInt(UTF8_String* string);

/**
 * Checks if a UTF-8 string is equal to a null-terminated C string.
 * @param s1 The UTF-8 string.
 * @param s2 The null-terminated C string.
 * @return true if the strings are equal, false otherwise.
 */
bool UTF8_equalStr(UTF8_String* s1, const char* s2);

/**
 * Deletes a UTF-8 slice and frees associated resources.
 * @param slice A pointer to the UTF-8 slice to be deleted.
 */
void UTF8_sliceDelete(UTF8_Slice* slice);

#endif