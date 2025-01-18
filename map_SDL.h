//
// Created by marc on 15/03/24.
//

#ifndef VIDEO_GAME_07_MAP_SDL_H
#define VIDEO_GAME_07_MAP_SDL_H

#include <SDL2/SDL.h>
#include "map.h"

#define BLOC_SIZE 32
#define OFFSET_MAP_X 3808
#define OFFSET_MAP_Y 5440

struct _Block{
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* texture;
};

typedef struct _Block Block;

/**
 * Représente l'affichage de la map
 */
struct _MapSDL{
    int x;
    int y;
    Block*** blocks;
    Block** blocks_object;
    Map* map;
    Map* object;
    int nb_objects;
    int nb_type_objects;
    int* objects_width;
    int* objects_height;
};

typedef struct _MapSDL MapSDL;

/**
 * Crée la map graphique
 * @param map La map
 * @param object Les objets
 * @return La map graphique
 */
MapSDL* create_MapSDL(Map* map, Map* object, int* objects_width, int* objects_height, int nb_objects, int nb_type_objects);

/**
 * Renvoie la map (partie qui contient les données) de la map graphique correspondante
 * @param mapSDL La map graphique
 * @return La map
 */
Map* map(MapSDL* mapSDL);

/**
 * Renvoie la map d'objet
 * @param mapSdl La map graphique
 * @return La map d'objet
 */
Map* map_objet(MapSDL* mapSdl);

/**
 * Renvoie la matrice de blocks
 * @param mapSdl La map graphique
 * @return La matrice de blocks
 */
Block*** blocks(MapSDL* mapSdl);

/**
 * Renvoie la matrice de blocks pour les objets
 * @param mapSdl La map graphique
 * @return La matrice de blocks d'objets
 */
Block** blocks_objects(MapSDL* mapSdl);

/**
 * Crée un block
 * @param i Sa position sur la i-ème ligne de la matrice
 * @param j Sa position sur la j_ème colonne de la matrice
 * @param val La valeur correspondante dans la matrice
 * @param texture La texture associé au monde
 * @return Le block correctement crée
 */
Block* create_block(int i,int j,int val,SDL_Texture* texture);

Block* create_block_object(int i,int j,int val, SDL_Renderer* renderer, MapSDL* mapSdl);

/**
 * Crée tout les blocks du monde
 * @param mapSDL Le monde graphique
 * @param texture La texture du monde
 * @param texture La texture des objets

 */
void init_Blocks(MapSDL* mapSDL, SDL_Texture* texture, SDL_Renderer* renderer);

/**
 * Rendu des blocks sur l'écran
 * @param mapSdl La map graphique qui contient la matrice de blocks représentant le monde
 * @param x L'opposé de la position x à laquelle le personnage se déplace
 * @param y L'opposé de la position y à laquelle le personnage se déplace
 */
void update_position_map(MapSDL* mapSDL, int x, int y);

/**
 * Rendu des blocks sur l'écran
 * @param mapSdl La map graphique qui contient la matrice de blocks représentant le monde
 * @param renderer Le renderer
 */
void render_blocks(MapSDL* mapSdl, SDL_Renderer* renderer);

/**
 * Libère un block de la mémoire
 * @param b Le block que l'on veut désallouer
 */
void free_block(Block* b);

/**
 * Libére tout les blocks de la mémoire
 * @param mapSDL Le monde graphique qui contient la matrice de blocks
 */
void free_blocks(MapSDL* mapSDL);

/**
 * Libère tout les blocks d'objets de la mémoire
 * @param mapSdl La map graphique
 */
void free_blocks_objects(MapSDL* mapSdl);

/**
 * Libère la map graphique de la mémoire
 * @param mapSdl La map graphique
 */
void free_mapSDL(MapSDL* mapSdl);

#endif //VIDEO_GAME_07_MAP_SDL_H
