#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "fonctions_SDL.h"

/**
 * Fonction permettant d'initialiser les différents élèments SDL lors
 * du lancement du jeu
 * SDL_Window** window: fenêtre de jeu
 * SDL_Renderer** renderer: le renderer
 * int w: La largeur de la fenêtre
 * int h: La hauteur de la fenêtre
 * Renvoie 0 si tout a été correctement initialisé
 * Renvoie -1 sinon
*/
int init(SDL_Window** window, SDL_Renderer** renderer, int w, int h){
    //*window = SDL_CreateWindow("SDL Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_FULLSCREEN);
    *window = SDL_CreateWindow("SDL Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_SHOWN);
    if(*window == NULL){
        fprintf(stderr, "Erreur SDL_Window : %s", SDL_GetError());
        return -1;
    }
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if(*renderer == NULL){
        fprintf(stderr, "Erreur SDL_Renderer : %s", SDL_GetError());
        return -1;
    }
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 150);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);

    return 0;
}

/**
 * Fonction permettant de charger une image
 * const char path[]: chemin vers l'image que l'on souhaite charger
 * SDL_Renderer* renderer: le renderer
 * Renvoie la texture de l'image que l'on a chargé
*/
SDL_Texture* load_image(const char path[], SDL_Renderer* renderer){
    SDL_Surface* tmp = NULL;
    SDL_Texture* texture = NULL;
    tmp = IMG_Load(path);
    if(!tmp){
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL; 
    }
    texture = SDL_CreateTextureFromSurface(renderer,tmp);
    SDL_FreeSurface(tmp);
    if(texture == NULL){
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

/**
 * Donne la largeur d'une texture donnée
 * SDL_Texture* texture: La texture dont on veut calculer la largeur
 * Retourne la valeur de la largeur de la texture
*/
int get_width(SDL_Texture* texture){
    int width;
    SDL_QueryTexture(texture,NULL,NULL,&width,NULL);
    return width;
}

/**
 * Donne la hauteur d'une texture donnée
 * SDL_Texture* texture: La texture dont on veut calculer la hauteur
 * Retourne la valeur de la hauteur de la texture
*/
int get_height(SDL_Texture* texture){
    int height;
    SDL_QueryTexture(texture,NULL,NULL,NULL,&height);
    return height;
}
