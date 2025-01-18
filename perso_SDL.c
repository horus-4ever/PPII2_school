#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fonctions_SDL.h"
#include "perso_SDL.h"
#include "map.h"
#include "map_SDL.h"
#include "quest/character.h"

/**
 * Initialise le personnage.
 * 
 * @param x Position en x sur l'écran.
 * @param y Position en y sur l'écran.
 * @param width Largeur du personnage dans le jeu.
 * @param height Hauteur du personnage dans le jeu.
 * @param path Chemin relatif vers l'image représentant le personnage.
 * @param renderer Le renderer SDL pour le rendu.
 * @param columns Nombre de colonnes dans la feuille de sprite du personnage.
 * @param lines Nombre de lignes dans la feuille de sprite du personnage.
 * @return Le personnage initialisé.
 */
Perso_SDL* init_perso(int x, int y, int width, int height, int orientation, const char path[], SDL_Renderer* renderer, int columns, int lines){
    Perso_SDL* perso = (Perso_SDL*)malloc(sizeof(Perso_SDL));
    perso->x = x;
    perso->y = y;
    perso->width = width;
    perso->height = height;
    perso->orientation = orientation; 
    perso->sprite = load_image(path,renderer);
    int w = get_width_spritesheet(perso);
    int h = get_height_spritesheet(perso);
    perso->srcRect.x = 0;
    if (lines == 1){
        perso->srcRect.y = 0;
    }
    else {
        switch (orientation)
        {
        case 1:
            perso->srcRect.y = 3*h/lines;
            break;
        case 2:
            perso->srcRect.y = 4*h/lines;   
            break;
        case 3:
            perso->srcRect.y = 1*h/lines;
            break;
        case 4:
            perso->srcRect.y = 2*h/lines;
            break;
        default:
            break;
        }
    }
    
    perso->srcRect.w = w/columns;
    perso->srcRect.h = h/lines;
    perso->dstRect.x = perso->x - OFFSET_PERSO_X - OFFSET_MAP_X;
    perso->dstRect.y = perso->y - OFFSET_PERSO_Y - OFFSET_MAP_Y;
    perso->dstRect.w = width;
    perso->dstRect.h = perso->height;
    perso->lastMove = 0;
    return perso;
}

/**
 * Initialise les vêtements du personnage.
 * 
 * @param renderer Le renderer SDL pour le rendu.
 * @param cheveux Type de cheveux.
 * @param oeil Type d'yeux.
 * @param haut Type de haut.
 * @param pantalon Type de pantalon.
 * @return Un tableau de pointeurs Perso_SDL représentant les vêtements du personnage initialisés.
 */
void init_clothes_perso(Perso_SDL*** clothes_perso, SDL_Renderer* renderer, int cheveux, int oeil, int haut, int pantalon, int spawn_x, int spawn_y, int woman){
    if (*clothes_perso != NULL){
        free_clothes(*clothes_perso);
        *clothes_perso = NULL;
    }
    *clothes_perso = malloc(4*sizeof(Perso_SDL));
    
    char chemin_cheveux[50];
    char chemin_oeil[50];
    char chemin_haut[50];
    char chemin_pantalon[50];
    if (woman){
        sprintf(chemin_cheveux, "ressources/images/vetements/woman/c%d.png", cheveux);
        sprintf(chemin_oeil, "ressources/images/vetements/woman/o%d.png", oeil);  
        sprintf(chemin_haut, "ressources/images/vetements/woman/h%d.png", haut);
        sprintf(chemin_pantalon, "ressources/images/vetements/woman/p%d.png", pantalon);
    } else {
        sprintf(chemin_cheveux, "ressources/images/vetements/man/c%d.png", cheveux);
        sprintf(chemin_oeil, "ressources/images/vetements/man/o%d.png", oeil);  
        sprintf(chemin_haut, "ressources/images/vetements/man/h%d.png", haut);
        sprintf(chemin_pantalon, "ressources/images/vetements/man/p%d.png", pantalon);
    }

    (*clothes_perso)[0] = init_perso(spawn_x,spawn_y,SIZE_PERSO,SIZE_PERSO,3, chemin_cheveux,renderer, 8,5);
    (*clothes_perso)[2] = init_perso(spawn_x,spawn_y,SIZE_PERSO,SIZE_PERSO,3,chemin_haut,renderer, 8,5);
    (*clothes_perso)[3] = init_perso(spawn_x,spawn_y,SIZE_PERSO,SIZE_PERSO,3,chemin_pantalon,renderer, 8,5);
    (*clothes_perso)[1] = init_perso(spawn_x,spawn_y,SIZE_PERSO,SIZE_PERSO,3,chemin_oeil,renderer, 8,5);
}

/**
 * Retourne la largeur de la feuille de sprite du personnage.
 * 
 * @param perso Le personnage dont on veut connaître la largeur de la feuille de sprite.
 * @return La largeur de la feuille de sprite du personnage.
 */
int get_width_spritesheet(Perso_SDL* perso){
    return get_width(perso->sprite);
}

/**
 * Retourne la hauteur de la feuille de sprite du personnage.
 * 
 * @param perso Le personnage dont on veut connaître la hauteur de la feuille de sprite.
 * @return La hauteur de la feuille de sprite du personnage.
 */
int get_height_spritesheet(Perso_SDL* perso){
    return get_height(perso->sprite);
}

/**
 * Vérifie si le joueur est en collision avec un élément de la carte lorsqu'il tente de se déplacer.
 * 
 * @param x Nouvelle position en x du joueur.
 * @param y Nouvelle position en y du joueur.
 * @param map La carte du jeu.
 * @param blocks Tableau de blocs de la carte.
 * @param blocks_object Tableau de blocs d'objets de la carte.
 * @return 1 si le joueur est en collision, sinon 0.
 */
int collision_bordure_map(int x, int y, Map* map, int nb){
    int block_x = x/BLOC_SIZE;
    if(block_x >= height_map(map) || block_x < 0){
        return 1;
    }
    int block_y = y/BLOC_SIZE;
    if(block_y >= length_map(map) ||block_y < 0){
        return 1;
    }
    int** mat = matrix_map(map);
    if(mat[block_x][block_y] != 0){
        return 1;
    }
    else{
        if (nb == 0){
            return collision_bordure_map(x-1, y, map, 1);
        }
        return 0;
    }
}

int collision_npc(int x, int y, ArrayList* characters, int nb){
    for (int i = 0; i <(int)ArrayList_length(characters) ; i++){
        Character* character = (Character*)ArrayList_get(characters, i);
        int x_char = character->sdlPerso->x;
        int y_char = character->sdlPerso->y;
        if (x-1 < x_char +3*BLOC_SIZE && x>= x_char+BLOC_SIZE && y >= y_char + 2*BLOC_SIZE && y < y_char + 5*BLOC_SIZE){
            return 1;
        }
    }
    return 0;
}

/**
 * Met à jour la position du joueur en vérifiant les collisions au préalable.
 * 
 * @param perso Le personnage.
 * @param clothes Tableau de pointeurs Perso_SDL représentant les vêtements du personnage.
 * @param x Nouvelle coordonnée en x.
 * @param y Nouvelle coordonnée en y.
 * @param mapSdl La carte graphique.
 */
int update_position_joueur(Perso_SDL* perso, Perso_SDL** clothes, int x, int y,MapSDL* mapSdl, ArrayList* characters){
    int taille_map_x = length_map(map(mapSdl))*BLOC_SIZE;
    int taille_map_y = height_map(map(mapSdl))*BLOC_SIZE;
    int w = get_width_spritesheet(perso);
    int h = get_height_spritesheet(perso);
    int collision_bordure = 0;
    int x_col_map = x + perso->width/2, y_col_map = y + perso->height-1;
    int ajustement_hitbox = (int)perso->width*0.25;
    int map_x = 0, map_y=0;
    //printf("x_map : %d, y_map : %d\n", perso->x, perso->y);fflush(stdout);
    
    // On va vers la gauche
    if(perso->x > x){
        perso->orientation = 4;
        perso->srcRect.y = (h/5)*2;
        if (clothes != NULL) {
            for (int i = 0; i < 4 ; i++){
                clothes[i]->srcRect.y = (h/5)*2;
            }
        }
        if(x + ajustement_hitbox < 0){
            collision_bordure = 1;
        }
        map_x += PERSO_SPEED;
    }
    // On va vers la droite
    else if(perso->x < x){
        perso->orientation = 2;
        perso->srcRect.y = (h/5)*4;
        if (clothes != NULL) {
            for (int i = 0; i < 4 ; i++){
                clothes[i]->srcRect.y = (h/5)*4;
            }
        }
        if(x - ajustement_hitbox - OFFSET_PERSO_X > taille_map_x - perso->width){
            collision_bordure = 1;
        }
        map_x -= PERSO_SPEED;
    }
    // On va vers le haut
    else if(perso->y > y){
        perso->orientation = 1;
        perso->srcRect.y = (h/5)*3;
        if (clothes != NULL) {
            for (int i = 0; i < 4 ; i++){
                clothes[i]->srcRect.y = (h/5)*3;
            }
        }
        if(y < 0){
            collision_bordure = 1;
        }
        map_y += PERSO_SPEED;
    }
    // On va vers le bas
    else if(perso->y < y){
        perso->orientation = 3;
        perso->srcRect.y = (h/5);
        if (clothes != NULL) {
            for (int i = 0; i < 4 ; i++){
                clothes[i]->srcRect.y = (h/5);
            }
        }
        if(y - OFFSET_PERSO_Y > taille_map_y - perso->height){
            collision_bordure = 1;
        }
        map_y -= PERSO_SPEED;
    }
    //On fait un 360
    else if(perso->y == y && perso->x == x){
        perso->orientation = 1;
        perso->srcRect.y = 0;
        if (clothes != NULL) {
            for (int i = 0; i < 4 ; i++){
                clothes[i]->srcRect.y = 0;
            }
        }
    }

    perso->srcRect.x += (w/8);
    perso->srcRect.x %= w;
    if (clothes != NULL) {
        for (int i = 0; i < 4 ; i++){
            clothes[i]->srcRect.x += (w/8);
            clothes[i]->srcRect.x %= w;
        }
    }
    if(!collision_bordure && !collision_bordure_map(x_col_map,y_col_map, mapSdl->object, 0) && !collision_npc(x_col_map, y_col_map, characters, 0)){
        perso->x = x;
        perso->y = y;
        if (clothes != NULL) {
            for (int i = 0; i < 4 ; i++){
                clothes[i]->x = x;
                clothes[i]->y = y;
            }
        }
        update_position_map(mapSdl,map_x,map_y);
        return 0;
    }
    return 1;
}

/**
 * Réinitialise l'animation du personnage pour simuler l'arrêt.
 * 
 * @param perso Le personnage.
 */
void reset_animation_perso(Perso_SDL* perso){
    perso->srcRect.x = 0;
}

void handleDoor(Perso_SDL* perso, Perso_SDL** clothes, MapSDL* mapSDL){
    int x_map = (perso->x + perso->width/2)/BLOC_SIZE, y_map = (perso->y + perso->height-1)/BLOC_SIZE;
    int type_object;
    switch (perso->orientation)
    {
    case 1: //en haut
        type_object = mapSDL->object->m[x_map][y_map-1];
        if (type_object == -2){
            perso->y = perso->y - 7*BLOC_SIZE;
            for (int i = 0; i < 4 ; i++){
                clothes[i]->y = clothes[i]->y - 7*BLOC_SIZE;
            }
            update_position_map(mapSDL, 0, 7*BLOC_SIZE);
        }
        break;
    case 2: //à droite
        type_object = mapSDL->object->m[x_map+1][y_map];
        if (type_object == -2){
            perso->x = perso->x + 3*BLOC_SIZE;
            for (int i = 0; i < 4 ; i++){
                clothes[i]->x = clothes[i]->x + 3*BLOC_SIZE;
            }
            update_position_map(mapSDL, -3*BLOC_SIZE, 0);
        }
        break;
    case 3: //en bas
        type_object = mapSDL->object->m[x_map][y_map+1];
        if (type_object == -2 || type_object == 21 || type_object == 20){
            perso->y = perso->y + 7*BLOC_SIZE;
            for (int i = 0; i < 4 ; i++){
                clothes[i]->y = clothes[i]->y + 7*BLOC_SIZE;
            }
            update_position_map(mapSDL, 0, -7*BLOC_SIZE);
        }
        break;
    case 4: //à gauche
        type_object = mapSDL->object->m[x_map-2][y_map];
        if (type_object == -2){
            perso->x = perso->x - 3*BLOC_SIZE;
            for (int i = 0; i < 4 ; i++){
                clothes[i]->x = clothes[i]->x - 3*BLOC_SIZE;
            }
            update_position_map(mapSDL, 3*BLOC_SIZE, 0);
        }
        break;
    default:
        break;
    }
}

/**
 * Libère la mémoire allouée pour les sprites vestimentaires.
 * 
 * @param clothes Tableau de pointeurs Perso_SDL représentant les sprites vestimentaires.
 */
void free_clothes(Perso_SDL** clothes){
    for (int i = 0; i<4;i++){
        free_perso(clothes[i]);
    }
    free(clothes);
}

/**
 * Libère le personnage de l'espace mémoire.
 * 
 * @param perso Le personnage à libérer.
 */
void free_perso(Perso_SDL* perso){
    SDL_DestroyTexture(perso->sprite);
    free(perso);
}

int distance_perso(Perso_SDL* p1, Perso_SDL* p2) {
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

bool perso_se_font_face(Perso_SDL* perso, Perso_SDL* other) {
    if(perso->orientation == 1 && other->orientation == 3 && perso->y > other->y) {
        return true;
    } else if(perso->orientation == 3 && other->orientation == 1 && perso->y < other->y) {
        return true;
    } else if(perso->orientation == 2 && other->orientation == 4 && perso->x < other->x) {
        return true;
    } else if(perso->orientation == 4 && other->orientation == 2 && perso->x > other->x) {
        return true;
    }
    return false;
}