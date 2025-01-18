#include "astmusic.h"
#include <stdlib.h>

AST_Music* AST_Music_new(UTF8_String* name, Mix_Chunk* music) {
    AST_Music* result = malloc(sizeof(AST_Music));
    result->name = name;
    result->music = music;
    return result;
}

void AST_Music_delete(AST_Music* item) {
    free(item);
}