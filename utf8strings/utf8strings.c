#include "utf8strings.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

UTF8_String* UTF8_fromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    UTF8_String* result = UTF8_empty();
    char buffer[1024] = {0};
    while(!feof(file)) {
        size_t read = fread(buffer, sizeof(char), 1023, file);
        buffer[read] = '\0';
        UTF8_append(result, buffer);
    }
    return result;
}  

UTF8_String* UTF8_from(const char* string) {
    UTF8_String* result = malloc(sizeof(UTF8_String));
    result->size = strlen(string);
    result->length = UTF8_utf8length(string);
    result->data = calloc(1, result->size + 1);
    memcpy(result->data, string, result->size);
    return result;
}

UTF8_String* UTF8_empty() {
    UTF8_String* result = malloc(sizeof(UTF8_String));
    result->size = 0;
    result->length = 0;
    result->data = calloc(1, 1);
    return result;
}

bool UTF8_equal(UTF8_String const* s1, UTF8_String const* s2) {
    if(s1->size != s2->size) {
        return false;
    }
    return memcmp(s1->data, s2->data, s1->size) == 0;
}

bool UTF8_equalStr(UTF8_String* s1, const char* s2) {
    if(strlen(s2) != s1->size) {
        return false;
    }
    return memcmp(s1->data, s2, s1->size) == 0;
}

size_t UTF8_hash(UTF8_String const* string) {
    return string->size;
}

int UTF8_toInt(UTF8_String* string) {
    return atoi((char*) string->data);
}

UTF8_String* UTF8_copy(UTF8_String const* string) {
    UTF8_String* result = malloc(sizeof(UTF8_String));
    result->size = UTF8_size(string);
    result->length = UTF8_length(string);
    result->data = calloc(1, result->size + 1);
    memcpy(result->data, string->data, result->size);
    return result;
}

size_t UTF8_size(UTF8_String const* string) {
    return string->size;
}

size_t UTF8_length(UTF8_String const* string) {
    return string->length;
}

void UTF8_append(UTF8_String* string, const char* str) {
    size_t strLength = UTF8_utf8length(str);
    size_t strSize = strlen(str);
    size_t resultLength = strLength + UTF8_length(string);
    size_t resultSize = strSize + UTF8_size(string);
    uint8_t* resultData = realloc(string->data, resultSize + 1);
    if(resultData == NULL) {
        resultData = calloc(1, resultSize + 1);
        memcpy(resultData, string->data, string->size);
        free(string->data);
    }
    memcpy(resultData + string->size, str, strSize);
    resultData[resultSize] = '\0';
    string->data = resultData;
    string->length = resultLength;
    string->size = resultSize;
}

UTF8_String* UTF8_pop(UTF8_String* string) {
    UTF8_Slice* slice = UTF8_slice(string, 0, UTF8_length(string) - 1);
    UTF8_String* result = UTF8_toString(slice);
    UTF8_sliceDelete(slice);
    return result;
}

size_t UTF8_utf8length(const char* string) {
    size_t count = 0;
    while (*string) {
        count += (*string++ & 0xC0) != 0x80;
    }
    return count;
}

UTF8_Slice* UTF8_emptySlice() {
    UTF8_Slice* result = malloc(sizeof(UTF8_Slice));
    result->begin = 0;
    result->end = 0;
    result->parent = NULL;
    result->real_begin = 0;
    result->real_end = 0;
    return result;
}

bool UTF8_isEmptySlice(const UTF8_Slice* slice) {
    return slice->begin == slice->end;
}

UTF8_Slice* UTF8_slice(UTF8_String const* string, size_t begin, size_t end) {
    size_t length = UTF8_length(string);
    if(begin >= length || end <= begin) {
        return UTF8_emptySlice();
    }
    if(end > UTF8_length(string)) {
        end = UTF8_length(string) - 1;
    }
    const char* str = UTF8_str(string);
    int begin_cpy = begin;
    int end_cpy = end;
    int total = end - begin;
    int real_begin = 0;
    int real_end = 0;
    int count = 0;
    while(*str && begin) {
        begin -= (*str & 0xC0) != 0x80;
        str += 1;
        count += 1;
    }
    real_begin = count;
    while(*str && total) {
        total -= (*str & 0xC0) != 0x80;
        str += 1;
        count += 1;
    }
    while(*str && (*str & 0xC0) == 0x80) {
        str += 1;
        count += 1;
    }
    real_end = count - 1;
    UTF8_Slice* result = malloc(sizeof(UTF8_Slice));
    result->begin = begin_cpy;
    result->end = end_cpy;
    result->real_begin = real_begin;
    result->real_end = real_end;
    result->parent = string;
    return result;
}

UTF8_String* UTF8_toString(const UTF8_Slice* slice) {
    if(UTF8_isEmptySlice(slice)) {
        return UTF8_empty();
    }
    UTF8_String* result = malloc(sizeof(UTF8_String));
    result->length = UTF8_sliceLength(slice);
    result->size = UTF8_sliceSize(slice);
    result->data = calloc(1, result->size + 1);
    memcpy(result->data, slice->parent->data + slice->real_begin, result->size);
    return result;
}

const char* UTF8_slicePtr(const UTF8_Slice* slice) {
    return (char*)slice->parent->data + slice->real_begin;
}

size_t UTF8_sliceLength(const UTF8_Slice* slice) {
    return slice->end - slice->begin;
}

size_t UTF8_sliceSize(const UTF8_Slice* slice) {
    return slice->real_end - slice->real_begin + 1;
}

void UTF8_delete(UTF8_String* string) {
    free(string->data);
    string->length = 0;
    string->size = 0;
    free(string);
}

void UTF8_sliceDelete(UTF8_Slice* slice) {
    free(slice);
}

char* UTF8_str(const UTF8_String* string) {
    return (char*)string->data;
}

void UTF8_appendChar(UTF8_String* string, char c) {
    char tab[2] = {c, '\0'};
    UTF8_append(string, tab);
}