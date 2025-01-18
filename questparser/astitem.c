#include "astitem.h"
#include <stdlib.h>

AST_Item* AST_Item_new(UTF8_String* name, UTF8_String* path, bool found) {
    AST_Item* result = malloc(sizeof(AST_Item));
    result->name = name;
    result->path = path;
    result->found = found;
    return result;
}

void AST_Item_delete(AST_Item* item) {
    UTF8_delete(item->name);
    UTF8_delete(item->path);
    free(item);
}