#ifndef ITEMS_SDL_H
#define ITEMS_SDL_H

#include "perso_SDL.h"
#include "datastructures/array.h"
#include "utf8strings/utf8strings.h"
#include <SDL2/SDL.h>

#define SIZE_ITEMS 85


/**
 * Représente les items du joueur
*/
struct _Items{
    int nb_items; /**Nombre d'items à trouver*/
    int* found; /**Tableau d'entiers valant 1 si l'items a été trouvé, 0 sinon*/
    UTF8_String** names; /** Tableau des noms des items */
    SDL_Rect* srcRect; /**Tableau des rectangles source des items*/
    SDL_Rect* dstRect; /**Tableau des rectangles destination des items*/
    SDL_Texture** sprite; /**Tableau des sprites des items*/
};

typedef struct _Items Items;

/**
 * Initialise le type Items
 * @param renderer Le renderer SDL pour le rendu.
 * @param nb_items Le nombre d'éléments à initialiser.
 * @return Un pointeur vers une nouvelle instance de la structure Items, ou NULL en cas d'échec.
 */
Items* init_items(SDL_Renderer* renderer, ArrayList* parsedItems);

/**
 * Affiche les items sur le rendu
 * @param items Les items
 * @param renderer Le renderer SDL pour le rendu.
*/
void render_items(Items* items, SDL_Renderer* renderer);

/**
 * Pour ajouter un item au perso
 * @param items Les items
 * @param i L'indice de l'item à ajouter
*/
void get_item(Items* items, int i);

/**
 * Libère les items de l'espace mémoire.
 * @param items Les items à libérer.
 */
void free_items(Items* items);

/**
 * Récupère l'indice d'un item selon son nom
*/
int index_of_item(Items* items, UTF8_String* name);


#endif