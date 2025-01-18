#include "font.h"
#include "utils.h"

WDG_Font* WDG_Font_new(const char* str, unsigned int fontSize) {
    WDG_Font* result = calloc(1, sizeof(WDG_Font));
    result->name = UTF8_from(str);
    result->size = fontSize;
    return result;
}

size_t WDG_Font_hash(WDG_Font* font) {
    return UTF8_hash(font->name) + font->size;
}

void WDG_Font_delete(WDG_Font* font) {
    UTF8_delete(font->name);
    free(font);
}

bool WDG_Font_equal(WDG_Font* a, WDG_Font* b) {
    return a->size == b->size && UTF8_equal(a->name, b->name);
}

WDG_FontManager* WDG_FontManager_new(void) {
    WDG_FontManager* result = calloc(1, sizeof(WDG_FontManager));
    WDG_FontManager_init(result);
    return result;
}

void WDG_FontManager_init(WDG_FontManager* manager) {
    manager->hashmap = HashMap_new(WDG_Font_delete, TTF_CloseFont, WDG_Font_equal, WDG_Font_hash);
}

TTF_Font* WDG_FontManager_get(WDG_FontManager* manager, const char* name, int fontSize) {
    WDG_Font *font = WDG_Font_new(name, fontSize);
    TTF_Font* result = HashMap_get(manager->hashmap, font);
    if(result != NULL) {
        WDG_Font_delete(font);
        return result;
    }
    UTF8_String* path = UTF8_from(FONT_PATH);
    UTF8_append(path, "/");
    UTF8_append(path, name);
    result = TTF_OpenFont(UTF8_str(path), fontSize);
    if(result == NULL) {
        WDG_SDLABORT(-1);
    }
    HashMap_set(manager->hashmap, font, result);
    UTF8_delete(path);
    return result;
}

void WDG_FontManager_delete(WDG_FontManager* manager) {
    HashMap_delete(manager->hashmap);
    free(manager);
}