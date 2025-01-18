#ifndef AST_ITEM_H
#define AST_ITEM_H

#include "../utf8strings/utf8strings.h"
#include <stdbool.h>

typedef struct {
    UTF8_String* name;
    UTF8_String* path;
    bool found;
} AST_Item;

AST_Item* AST_Item_new(UTF8_String* name, UTF8_String* path, bool found);
void AST_Item_delete(AST_Item* item);

#endif