//
// Created by marc on 15/03/24.
//

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "assert.h"
#include "map_SDL.h"
#include "fonctions_SDL.h"
#include "perso_SDL.h"
#include "menu.h"

void test_get_info_map(){
    int l, h;
    char* nom_fichier = "tests/test1.txt";
    if(access(nom_fichier, F_OK) == -1){
        printf("Nom de fichier %s inexistant.\n",nom_fichier);fflush(stdout);
    }
    FILE* f = fopen(nom_fichier,"r");
    assert(0 == get_info_map(f,&l,&h));
    assert(l == 20 && h == 10);
}

void test_create_map1(){
    Map* m = create_map(20,10);
    assert(length_map(m)==20);
    assert(height_map(m)==10);
    free_map(m);
}

void test_read_matrix(){
    Map* map = NULL;
    init_map("tests/test3.txt",&map, 0);
    int** m = matrix_map(map);
    int i = WIDTH/BLOC_SIZE, j = HEIGHT/BLOC_SIZE;
    assert(m[i][j]==1 && m[i+1][j]==2);
    assert(m[i][j+1]==3 && m[i+1][j+1]==4);
    assert(m[i][j+2]==5 && m[i+1][j+2]==6);
    free_map(map);
}

void test_create_bloc(){
    Block* b = create_block(0,0,0,NULL);
    free_block(b);
}

void test_compute_matrix_size(){
    int l = 9, h = 13;
    compute_matrix_size(&l,&h);
    assert(l == 9+2*(HEIGHT/BLOC_SIZE));
    assert(h == 13+2*(WIDTH/BLOC_SIZE));
}

// Sample deleter function
void sample_deleter(void* element) {
    free(element);
}

// Sample comparison function
bool sample_compare(void* a, void* b) {
    return strcmp((char*)a, (char*)b) == 0;
}

size_t string_hash(void* key) {
    const char* str = (const char*)key;
    size_t hash = 0;
    while (*str) {
        hash = (hash * 31) + (unsigned char)*str++;
    }
    return hash;
}

bool string_compare(void* a, void* b) {
    return strcmp((const char*)a, (const char*)b) == 0;
}

void string_deleter(void* data) {
    free(data);
}

// Helper function to create a dynamically allocated string
char* create_string(char* str) {
    size_t len = strlen(str) + 1;
    char* copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

void test_ArrayList_new() {
    ArrayList* list = ArrayList_new(sample_deleter);
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->deleter == sample_deleter);
    ArrayList_delete(list);
}

void test_ArrayList_init() {
    ArrayList list;
    ArrayList_init(&list, sample_deleter);
    assert(list.length == 0);
    assert(list.deleter == sample_deleter);
    ArrayList_clear(&list); // Cleaning up
}

void test_ArrayList_append_and_length() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");

    ArrayList_append(list, value1);
    assert(ArrayList_length(list) == 1);
    ArrayList_append(list, value2);
    assert(ArrayList_length(list) == 2);

    ArrayList_delete(list);
}

void test_ArrayList_append_first() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");

    ArrayList_append_first(list, value1);
    assert(ArrayList_length(list) == 1);
    ArrayList_append_first(list, value2);
    assert(ArrayList_length(list) == 2);
    assert(strcmp(ArrayList_head(list), "value2") == 0);

    ArrayList_delete(list);
}

void test_ArrayList_pop() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");

    ArrayList_append(list, value1);
    ArrayList_append(list, value2);
    assert(ArrayList_length(list) == 2);

    void* popped_value = ArrayList_pop(list);
    assert(strcmp(popped_value, "value2") == 0);
    free(popped_value);
    assert(ArrayList_length(list) == 1);

    popped_value = ArrayList_pop(list);
    assert(strcmp(popped_value, "value1") == 0);
    free(popped_value);
    assert(ArrayList_length(list) == 0);

    ArrayList_delete(list);
}

void test_ArrayList_remove() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");
    char* value3 = create_string("value3");

    ArrayList_append(list, value1);
    ArrayList_append(list, value2);
    ArrayList_append(list, value3);
    assert(ArrayList_length(list) == 3);

    ArrayList_remove(list, value2);
    assert(ArrayList_length(list) == 2);
    assert(strcmp(ArrayList_get(list, 0), "value1") == 0);
    assert(strcmp(ArrayList_get(list, 1), "value3") == 0);

    ArrayList_delete(list);
}

void test_ArrayList_removeAt() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");
    char* value3 = create_string("value3");

    ArrayList_append(list, value1);
    ArrayList_append(list, value2);
    ArrayList_append(list, value3);
    assert(ArrayList_length(list) == 3);

    ArrayList_removeAt(list, 1);
    assert(ArrayList_length(list) == 2);
    assert(strcmp(ArrayList_get(list, 0), "value1") == 0);
    assert(strcmp(ArrayList_get(list, 1), "value3") == 0);

    ArrayList_delete(list);
}

void test_ArrayList_get() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");

    ArrayList_append(list, value1);
    ArrayList_append(list, value2);
    assert(ArrayList_length(list) == 2);

    assert(strcmp(ArrayList_get(list, 0), "value1") == 0);
    assert(strcmp(ArrayList_get(list, 1), "value2") == 0);

    ArrayList_delete(list);
}

void test_ArrayList_find() {
    ArrayList* list = ArrayList_new(sample_deleter);
    char* value1 = create_string("value1");
    char* value2 = create_string("value2");

    ArrayList_append(list, value1);
    ArrayList_append(list, value2);

    void* found_value = ArrayList_find(list, "value2", sample_compare);
    assert(found_value != NULL);
    assert(strcmp(found_value, "value2") == 0);

    found_value = ArrayList_find(list, "value3", sample_compare);
    assert(found_value == NULL);

    ArrayList_delete(list);
}

void test_UTF8_from() {
    UTF8_String* string = UTF8_from("hello");
    assert(string != NULL);
    assert(UTF8_size(string) == 5);
    assert(UTF8_length(string) == 5);
    assert(strcmp(UTF8_str(string), "hello") == 0);
    UTF8_delete(string);
}

void test_UTF8_empty() {
    UTF8_String* string = UTF8_empty();
    assert(string != NULL);
    assert(UTF8_size(string) == 0);
    assert(UTF8_length(string) == 0);
    UTF8_delete(string);
}

void test_UTF8_copy() {
    UTF8_String* original = UTF8_from("hello");
    UTF8_String* copy = UTF8_copy(original);
    assert(copy != NULL);
    assert(UTF8_size(copy) == 5);
    assert(UTF8_length(copy) == 5);
    assert(UTF8_equal(original, copy));
    UTF8_delete(original);
    UTF8_delete(copy);
}

void test_UTF8_equal() {
    UTF8_String* s1 = UTF8_from("test");
    UTF8_String* s2 = UTF8_from("test");
    UTF8_String* s3 = UTF8_from("different");
    assert(UTF8_equal(s1, s2));
    assert(!UTF8_equal(s1, s3));
    UTF8_delete(s1);
    UTF8_delete(s2);
    UTF8_delete(s3);
}

void test_UTF8_append() {
    UTF8_String* string = UTF8_from("hello");
    UTF8_append(string, " world");
    assert(UTF8_size(string) == 11);
    assert(UTF8_length(string) == 11);
    assert(strcmp(UTF8_str(string), "hello world") == 0);
    UTF8_delete(string);
}

void test_UTF8_pop() {
    UTF8_String* string = UTF8_from("hello");
    UTF8_String* popped = UTF8_pop(string);
    assert(strcmp(UTF8_str(popped), "hell") == 0);
    assert(UTF8_size(string) == 5);
    assert(UTF8_length(string) == 5);
    assert(strcmp(UTF8_str(string), "hello") == 0);
    UTF8_delete(string);
    UTF8_delete(popped);
}

void test_UTF8_appendChar() {
    UTF8_String* string = UTF8_from("hello");
    UTF8_appendChar(string, '!');
    assert(UTF8_size(string) == 6);
    assert(UTF8_length(string) == 6);
    assert(strcmp(UTF8_str(string), "hello!") == 0);
    UTF8_delete(string);
}

void test_UTF8_toInt() {
    UTF8_String* string = UTF8_from("12345");
    int value = UTF8_toInt(string);
    assert(value == 12345);
    UTF8_delete(string);
}

void test_UTF8_equalStr() {
    UTF8_String* s1 = UTF8_from("test");
    assert(UTF8_equalStr(s1, "test"));
    assert(!UTF8_equalStr(s1, "different"));
    UTF8_delete(s1);
}

void test_HashMap_new() {
    HashMap* hashmap = HashMap_new(string_deleter, string_deleter, string_compare, string_hash);
    assert(hashmap != NULL);
    assert(hashmap->size > 0);
    assert(hashmap->nb == 0);
    HashMap_delete(hashmap);
}

void test_HashMap_set_get() {
    HashMap* hashmap = HashMap_new(string_deleter, string_deleter, string_compare, string_hash);
    
    char* key1 = create_string("key1");
    char* value1 = create_string("value1");
    HashMap_set(hashmap, key1, value1);
    
    char* result = (char*)HashMap_get(hashmap, key1);
    assert(result != NULL);
    assert(strcmp(result, "value1") == 0);
    
    HashMap_delete(hashmap);
}

void test_HashMap_clear() {
    HashMap* hashmap = HashMap_new(string_deleter, string_deleter, string_compare, string_hash);
    
    char* key1 = create_string("key1");
    char* value1 = create_string("value1");
    HashMap_set(hashmap, key1, value1);
    
    HashMap_clear(hashmap);
    assert(hashmap->nb == 0);
    
    char* result = (char*)HashMap_get(hashmap, key1);
    assert(result == NULL);
    
    HashMap_delete(hashmap);
}

void test_HashMap_delete() {
    HashMap* hashmap = HashMap_new(string_deleter, string_deleter, string_compare, string_hash);
    
    char* key1 = create_string("key1");
    char* value1 = create_string("value1");
    HashMap_set(hashmap, key1, value1);
    
    HashMap_delete(hashmap);  // No assert, just ensuring no memory leaks or crashes
}

int main(void){
    test_get_info_map();
    test_create_map1();
    test_read_matrix();
    test_create_bloc();
    test_compute_matrix_size();
    // test for the arrays
    test_ArrayList_new();
    test_ArrayList_init();
    test_ArrayList_append_and_length();
    test_ArrayList_append_first();
    test_ArrayList_pop();
    test_ArrayList_remove();
    test_ArrayList_removeAt();
    test_ArrayList_get();
    test_ArrayList_find();
    // tests for the strings
    test_UTF8_from();
    test_UTF8_empty();
    test_UTF8_copy();
    test_UTF8_equal();
    test_UTF8_append();
    test_UTF8_pop();
    test_UTF8_appendChar();
    test_UTF8_toInt();
    test_UTF8_equalStr();
    // tests for the hashmap
    test_HashMap_new();
    test_HashMap_set_get();
    test_HashMap_clear();
    test_HashMap_delete();

    printf("All passed\n");
    return 0;
}