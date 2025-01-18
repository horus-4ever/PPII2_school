#ifndef AST_MUSIC
#define AST_MUSIC

#include "../utf8strings/utf8strings.h"
#include <SDL2/SDL_mixer.h>

typedef struct {
    UTF8_String* name;
    Mix_Chunk* music;
} AST_Music;

AST_Music* AST_Music_new(UTF8_String* name, Mix_Chunk* music);
void AST_Music_delete(AST_Music* item);

#endif