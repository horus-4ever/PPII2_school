#ifndef FONCTIONS_SDL_H
#define FONCTIONS_SDL_H

// Largeur de la fenêtre de jeu
#define WIDTH 1280

// Hauteur de la fenêtre de jeu
#define HEIGHT 896

#define FPS 60.0

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
int init(SDL_Window** window, SDL_Renderer** renderer, int w, int h);

/**
 * Fonction permettant de charger une image
 * const char path[]: chemin vers l'image que l'on souhaite charger
 * SDL_Renderer* renderer: le renderer
 * Renvoie la texture de l'image que l'on a chargé
*/
SDL_Texture* load_image(const char path[], SDL_Renderer* renderer);

/**
 * Donne la largeur d'une texture donnée
 * SDL_Texture* texture: La texture dont on veut calculer la largeur
 * Retourne la valeur de la largeur de la texture
*/
int get_width(SDL_Texture* texture);

/**
 * Donne la hauteur d'une texture donnée
 * SDL_Texture* texture: La texture dont on veut calculer la hauteur
 * Retourne la valeur de la hauteur de la texture
*/
int get_height(SDL_Texture* texture);

#endif
