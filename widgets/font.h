#ifndef WDG_FONT_H
#define WDG_FONT_H

#include "../utf8strings/utf8strings.h"
#include "../datastructures/hashmap.h"
#include <SDL2/SDL_ttf.h>

#define FONT_PATH "ressources/fonts"
#define DEFAULT_FONT "zpix.ttf"
#define DEFAULT_FONTSIZE 25

/**
 * @brief Represents a font.
 */
typedef struct {
    UTF8_String* name;     ///< The name of the font.
    unsigned int size;     ///< The size of the font.
} WDG_Font;

/**
 * @brief Creates a new font.
 * @param str The name of the font.
 * @param fontSize The size of the font.
 * @return A pointer to the newly created WDG_Font object.
 */
WDG_Font* WDG_Font_new(const char* str, unsigned int fontSize);

/**
 * @brief Deletes a font and frees its resources.
 * @param font The WDG_Font object to be deleted.
 */
void WDG_Font_delete(WDG_Font* font);

/**
 * @brief Computes the hash value of a font.
 * @param font The font object.
 * @return The hash value of the font.
 */
size_t WDG_Font_hash(WDG_Font* font);

/**
 * @brief Checks if two fonts are equal.
 * @param a The first font.
 * @param b The second font.
 * @return True if the fonts are equal, otherwise false.
 */
bool WDG_Font_equal(WDG_Font* a, WDG_Font* b);

/**
 * @brief Manages fonts.
 */
typedef struct {
    HashMap* hashmap;  ///< The hashmap storing fonts.
} WDG_FontManager;

/**
 * @brief Creates a new font manager.
 * @return A pointer to the newly created WDG_FontManager object.
 */
WDG_FontManager* WDG_FontManager_new(void);

/**
 * @brief Initializes a font manager.
 * @param manager The font manager to be initialized.
 */
void WDG_FontManager_init(WDG_FontManager* manager);

/**
 * @brief Gets a font from the font manager.
 * @param manager The font manager.
 * @param name The name of the font.
 * @param fontSize The size of the font.
 * @return The TTF_Font object corresponding to the specified name and size.
 */
TTF_Font* WDG_FontManager_get(WDG_FontManager* manager, const char* name, int fontSize);

/**
 * @brief Deletes a font manager and frees its resources.
 * @param manager The font manager to be deleted.
 */
void WDG_FontManager_delete(WDG_FontManager* manager);

#endif