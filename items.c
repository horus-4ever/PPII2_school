#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "items.h"
#include "perso_SDL.h"
#include "questparser/astitem.h"

/**
 * Initialise le type Items
 * @param renderer Le renderer SDL pour le rendu.
 * @param nb_items Le nombre d'éléments à initialiser.
 * @return Un pointeur vers une nouvelle instance de la structure Items, ou NULL en cas d'échec.
 */
Items* init_items(SDL_Renderer* renderer, ArrayList* parsedItems) {
    Items* items = (Items*)malloc(sizeof(Items));
    
    if (items == NULL) {
        printf("Erreur : échec d'allocation pour Items.\n");fflush(stdout);
        return NULL;
    }

    int nb_items = ArrayList_length(parsedItems);
    items->nb_items = nb_items;

    items->found = (int*)calloc(nb_items, sizeof(int));
    items->srcRect = (SDL_Rect*)malloc(nb_items * sizeof(SDL_Rect));
    items->dstRect = (SDL_Rect*)malloc(nb_items * sizeof(SDL_Rect));
    items->sprite = (SDL_Texture**)malloc(nb_items * sizeof(SDL_Texture*));
    items->names = (UTF8_String**) malloc(nb_items * sizeof(UTF8_String*));

    if (items->found == NULL || items->srcRect == NULL ||
        items->dstRect == NULL || items->sprite == NULL) {
        printf("Erreur : échec d'allocation pour les tableaux.\n");fflush(stdout);
        free(items->found);
        free(items->srcRect);
        free(items->dstRect);
        free(items->sprite);
        free(items);
        return NULL;
    }

    for (int i = 0; i < nb_items; ++i) {
        AST_Item* item = ArrayList_get(parsedItems, i);
        items->sprite[i] = load_image((char*) item->path->data, renderer);
        items->names[i] = UTF8_copy(item->name);
        items->found[i] = item->found;
        items->srcRect[i] = (SDL_Rect){(int) item->found * 85, 0, SIZE_ITEMS, SIZE_ITEMS};
        items->dstRect[i] = (SDL_Rect){25*(i+1) + SIZE_ITEMS*i, 25, SIZE_ITEMS, SIZE_ITEMS};
    }

    return items;
}

/**
 * Affiche les items sur le rendu
 * @param items Les items
 * @param renderer Le renderer SDL pour le rendu.
*/
void render_items(Items* items, SDL_Renderer* renderer){
    for (int i = 0; i<items->nb_items; i++){
        SDL_RenderCopy(renderer, items->sprite[i], &items->srcRect[i], &items->dstRect[i]);
    }   
}

/**
 * Pour ajouter un item au perso
 * @param items Les items
 * @param i L'indice de l'item à ajouter
*/
void get_item(Items* items, int i){
    items->found[i] = 1;
    items->srcRect[i] = (SDL_Rect){85, 0, 85, 85};
}

int index_of_item(Items* items, UTF8_String* name) {
    for(int i = 0 ; i < items->nb_items ; i += 1) {
        if(UTF8_equal(items->names[i], name)) {
            return i;
        }
    }
    return -1;
}

/**
 * Libère les items de l'espace mémoire.
 * @param items Les items à libérer.
 */
void free_items(Items* items) {
    if (items == NULL) return;

    free(items->found);
    free(items->srcRect);
    free(items->dstRect);
    for (int i = 0; i< items->nb_items; i++){
        SDL_DestroyTexture(items->sprite[i]);
    }
    for(int i = 0 ; i < items->nb_items ; i++){
        UTF8_delete(items->names[i]);
    }
    free(items->names);
    free(items->sprite);
    free(items);
}


