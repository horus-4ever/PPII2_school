//
// Created by marc on 15/03/24.
//

#include "map_SDL.h"
#include "perso_SDL.h"
#include <SDL2/SDL.h>

/**
 * Crée la map graphique
 * @param map La map
 * @param object La map d'objets
 * @return La map graphique
 */
MapSDL* create_MapSDL(Map* map, Map* object, int* object_width, int* object_height, int nb_objects, int nb_type_objects){
    MapSDL* mapSdl = (MapSDL*)malloc(sizeof(MapSDL));
    mapSdl->x = WIDTH;
    mapSdl->y = HEIGHT;
    mapSdl->map = map;
    mapSdl->object = object;
    mapSdl->objects_width = object_width;
    mapSdl->objects_height = object_height;
    mapSdl->nb_objects = nb_objects;
    mapSdl->nb_type_objects = nb_type_objects;
    mapSdl->blocks = (Block***)malloc(height_map(map)*sizeof(Block**));
    mapSdl->blocks_object = (Block**)malloc(nb_objects*sizeof(Block*));
    for(int i=0;i< height_map(map);i++){
        mapSdl->blocks[i] = (Block**)malloc(length_map(map)*sizeof(Block*));
    }
    return mapSdl;
}

/**
 * Renvoie la map (partie qui contient les données) de la map graphique correspondante
 * @param mapSDL La map graphique
 * @return La map
 */
Map* map(MapSDL* mapSDL){
    return mapSDL->map;
}

/**
 * Renvoie la map d'objet
 * @param mapSdl La map graphique
 * @return La map d'objet
 */
Map* map_objet(MapSDL* mapSdl){
    return mapSdl->object;
}

/**
 * Renvoie la matrice de blocks
 * @param mapSdl La map graphique
 * @return La matrice de blocks
 */
Block*** blocks(MapSDL* mapSdl){
    return mapSdl->blocks;
}

/**
 * Renvoie la matrice de blocks pour les objets
 * @param mapSdl La map graphique
 * @return La matrice de blocks d'objets
 */
Block** blocks_objects(MapSDL* mapSdl){
    return mapSdl->blocks_object;
}

/**
 * Crée un block
 * @param i Sa position sur la i-ème ligne de la matrice
 * @param j Sa position sur la j_ème colonne de la matrice
 * @param val La valeur correspondante dans la matrice
 * @param texture La texture associé au monde
 * @param type 0 si background, 1 si object
 * @return Le block correctement crée
 */
Block* create_block(int i,int j,int val,SDL_Texture* texture){
    Block* b = (Block*)malloc(sizeof(Block));
    int side_length = get_height(texture);
    SDL_Rect src = {side_length*val,0,side_length,side_length};
    b->src = src;
    SDL_Rect dst = {i*BLOC_SIZE - OFFSET_PERSO_X - OFFSET_MAP_X,j*BLOC_SIZE - OFFSET_PERSO_Y - OFFSET_MAP_Y,BLOC_SIZE,BLOC_SIZE};
    b->dst = dst;
    b->texture = texture;
    
    return b;
}

Block* create_block_object(int i,int j,int val, SDL_Renderer* renderer, MapSDL* mapSdl){
    Block* b = (Block*)malloc(sizeof(Block));
    char path_object[50];
    sprintf(path_object, "ressources/images/objects/%d.png", val);
    SDL_Texture* texture = load_image(path_object, renderer);
    SDL_Rect src = {0,0,get_width(texture),get_height(texture)};
    b->src = src;
    SDL_Rect dst = {i*BLOC_SIZE - OFFSET_PERSO_X - OFFSET_MAP_X,j*BLOC_SIZE - OFFSET_PERSO_Y - OFFSET_MAP_Y,mapSdl->objects_width[val-1]*BLOC_SIZE,mapSdl->objects_height[val-1]*BLOC_SIZE};
    b->dst = dst;
    b->texture = texture;
    return b;
}

/**
 * Crée tout les blocks du monde
 * @param mapSDL Le monde graphique
 * @param texture La texture du monde
 */
void init_Blocks(MapSDL* mapSDL, SDL_Texture* texture, SDL_Renderer* renderer){
    Block*** b = blocks(mapSDL);
    Block** bo = blocks_objects(mapSDL);

    int** m = matrix_map(map(mapSDL));
    int** mo = matrix_map(mapSDL->object);

    int k = 0;
    for(int i=0;i< length_map(map(mapSDL));i++){
        for(int j=0;j< height_map(map(mapSDL));j++){
            Block* block = create_block(j,i,m[j][i],texture);
            b[j][i] = block;
            if (mo[j][i] > 0){
                Block* block2 = create_block_object(j,i,mo[j][i], renderer, mapSDL);
                bo[k] = block2;
                k++;
            }
        }
    }
}


/**
 * Rendu des blocks sur l'écran
 * @param mapSdl La map graphique qui contient la matrice de blocks représentant le monde
 * @param x L'opposé de la position x à laquelle le personnage se déplace
 * @param y L'opposé de la position y à laquelle le personnage se déplace
 */
void update_position_map(MapSDL* mapSDL, int x, int y) {
    // Vérification des paramètres
    if (mapSDL == NULL || mapSDL->blocks == NULL || mapSDL->blocks_object == NULL) {
        printf("Erreur : la carte est vide.\n");
        return;
    }

    // Parcours des blocks pour mettre à jour les coordonnées
    for (int i = 0; i < mapSDL->map->h; i++) {
        for (int j = 0; j < mapSDL->map->l; j++) {
            if (mapSDL->blocks[i][j] != NULL) {
                mapSDL->blocks[i][j]->dst.x += x;
                mapSDL->blocks[i][j]->dst.y += y;
            }
        }
    }

    //Parcours des blocks_object pour mettre à jour les coordonnées
    for (int i = 0; i < mapSDL->nb_objects; i++) {
        if (mapSDL->blocks_object[i] != NULL) {
            mapSDL->blocks_object[i]->dst.x += x;
            mapSDL->blocks_object[i]->dst.y += y;
        }
    }
}

/**
 * Rendu des blocks sur l'écran
 * @param mapSdl La map graphique qui contient la matrice de blocks représentant le monde
 * @param renderer Le renderer
 */
void render_blocks(MapSDL* mapSdl, SDL_Renderer* renderer){
    Block*** b = blocks(mapSdl);
    Block** bo = blocks_objects(mapSdl);
    for(int i=0;i<height_map(map(mapSdl));i++){
        for(int j=0;j< length_map(map(mapSdl));j++){
            Block* block = b[i][j];
            SDL_RenderCopy(renderer,block->texture,&block->src,&block->dst);
        }
    }
    for (int i = 0; i< mapSdl->nb_objects ; i++){
        Block* block = bo[i];
        SDL_RenderCopy(renderer,block->texture,&block->src,&block->dst);
    }
}


/**
 * Libère un block de la mémoire
 * @param b Le block que l'on veut désallouer
 */
void free_block(Block* b){
    SDL_DestroyTexture(b->texture);
    free(b);
}

/**
 * Libére tout les blocks de la mémoire
 * @param mapSDL Le monde graphique qui contient la matrice de blocks
 */
void free_blocks(MapSDL* mapSDL){
    Block*** b = blocks(mapSDL);
    Map* m = map(mapSDL);
    for(int i=height_map(m)-1;i>=0 ;i--){
        for(int j=length_map(m)-1;j>=0;j--){
            free_block(b[i][j]);
        }
        free(b[i]);
    }
    free(b);
}

/**
 * Libère tout les blocks d'objets de la mémoire
 * @param mapSdl La map graphique
 */
void free_blocks_objects(MapSDL* mapSdl){
    Block** b = blocks_objects(mapSdl);
    for(int i=0 ; i < mapSdl->nb_objects;i++){
        free_block(b[i]);      
    }
    free(b);
}

/**
 * Libère la map graphique de la mémoire
 * @param mapSdl La map graphique
 */
void free_mapSDL(MapSDL* mapSdl){
    free_blocks(mapSdl);
    free_blocks_objects(mapSdl);
    free_map(map(mapSdl));
    free(mapSdl->objects_height);
    free(mapSdl->objects_width);
    free_map(map_objet(mapSdl));
    free(mapSdl);
}
