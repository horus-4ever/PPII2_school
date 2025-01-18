#ifndef PERSO_SDL_H
#define PERSO_SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "datastructures/array.h"
#include "fonctions_SDL.h"
#include "map_SDL.h"
#include "map.h"


// Taille du personnage
#define SIZE_PERSO BLOC_SIZE*4
#define SIZE_PERSO_MENU 500

// Vitesse du personnage
#define PERSO_SPEED 32

// Décallage en x du perso pour prendre en compte la bordure
#define OFFSET_PERSO_X WIDTH

// Décalle en y du perso pour prendre en compte la bordure
#define OFFSET_PERSO_Y HEIGHT

/**
 * Représente l'affichage du personnage
 */
struct _Perso_SDL{
    int x; /** La position en x dans le jeu du personnage */
    int y; /** La position en y dans le jeu du personnage */
    int width; /** La largeur du personnage dans le jeu */
    int height; /** La hauteur du personnage dans le jeu */
    SDL_Rect srcRect; /** La partie rendue dans le jeu de l'image correspondante au perso */
    SDL_Rect dstRect; /** L'affichage du sprite dans le jeu */
    SDL_Texture* sprite; /** La feuille de sprite du perso */
    int orientation; /**1 pour vers le haut, 2 pour droite, 3 pour bas et 4 pour gauche*/
    Uint32 lastMove; /** Temps du dernier déplacement */
};

typedef struct _Perso_SDL Perso_SDL;



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
Perso_SDL* init_perso(int x, int y, int width, int height, int orientation, const char path[], SDL_Renderer* renderer, int columns, int lines);

/**
 * Initialise les vêtements du personnage.
 * 
 * @param renderer Le renderer SDL pour le rendu.
 * @param cheveux Type de cheveux.
 * @param oeil Type d'yeux.
 * @param haut Type de haut.
 * @param pantalon Type de pantalon.
 */
void init_clothes_perso(Perso_SDL*** clothes_perso, SDL_Renderer* renderer, int cheveux, int oeil, int haut, int pantalon, int spawn_x, int spawn_y, int woman);

/**
 * Retourne la largeur de la feuille de sprite du personnage.
 * 
 * @param perso Le personnage dont on veut connaître la largeur de la feuille de sprite.
 * @return La largeur de la feuille de sprite du personnage.
 */
int get_width_spritesheet(Perso_SDL* perso);

/**
 * Retourne la hauteur de la feuille de sprite du personnage.
 * 
 * @param perso Le personnage dont on veut connaître la hauteur de la feuille de sprite.
 * @return La hauteur de la feuille de sprite du personnage.
 */
int get_height_spritesheet(Perso_SDL* perso);

/**
 * Vérifie si le joueur est en collision avec un élément de la carte lorsqu'il tente de se déplacer.
 * 
 * @param x Nouvelle position en x du joueur.
 * @param y Nouvelle position en y du joueur.
 * @param map La carte du jeu.
 * @param nb 0 si on regarde la collision avec le pixel gauche, 1 pour droite
 * @return 1 si le joueur est en collision, sinon 0.
 */
int collision_bordure_map(int x, int y, Map* map, int nb);


/**
 * Réinitialise l'animation du personnage pour simuler l'arrêt.
 * 
 * @param perso Le personnage.
 */
void reset_animation_perso(Perso_SDL* perso);

void handleDoor(Perso_SDL* perso, Perso_SDL** clothes, MapSDL* mapSDL);

/**
 * Libère la mémoire allouée pour les sprites vestimentaires dans un menu.
 * 
 * @param clothes Tableau de pointeurs Perso_SDL représentant les sprites vestimentaires.
 */
void free_clothes(Perso_SDL** clothes);


/**
 * Libère le personnage de l'espace mémoire.
 * 
 * @param perso Le personnage à libérer.
 */
void free_perso(Perso_SDL* perso);

/**
 * Calcule la distance entre deux personnages
 * @param p1 le premier personnage
 * @param p2 le second personnage
 * @return la distance entre les deux personnages
*/
int distance_perso(Perso_SDL* p1, Perso_SDL* p2);

int collision_npc(int x, int y, ArrayList* characters, int nb);

/**
 * Met à jour la position du joueur en vérifiant les collisions au préalable.
 * 
 * @param perso Le personnage.
 * @param clothes Tableau de pointeurs Perso_SDL représentant les vêtements du personnage.
 * @param x Nouvelle coordonnée en x.
 * @param y Nouvelle coordonnée en y.
 * @param mapSdl La carte graphique.
 */

int update_position_joueur(Perso_SDL* perso, Perso_SDL** clothes, int x, int y,MapSDL* mapSdl, ArrayList* characters);

/**
 * Indique si deux personnages se font face.
 * 
 * @param perso le premier personnage
 * @param other le second personnage
 * @return vrai si les personnages se font face
*/
bool perso_se_font_face(Perso_SDL* perso, Perso_SDL* other);

#endif
