#ifndef MENU_SDL_H
#define MENU_SDL_H

#include "perso_SDL.h"
#include "map_SDL.h"
#include "map.h"
#include "widgets/widgets.h"
#include <SDL2/SDL.h>


struct _Clothes_SDL{
    Perso_SDL** clothes_menu_woman;
    Perso_SDL** clothes_menu_man;
    int* cheveux;
    int* oeil;
    int* haut;
    int* pantalon;
};

typedef struct _Clothes_SDL Clothes_SLD;

struct Game;

/**
 * @brief Represents the game menu.
 */
typedef struct GameMenu {
    // Graphical elements
    SDL_Texture* background;        ///< Background texture.
    WDG_Button* buttonMan;          ///< Button for male character.
    WDG_Button* buttonWoman;        ///< Button for female character.
    WDG_Button* buttonSave;         ///< Button for saving game.
    WDG_Button** arrows_menu;       ///< Menu arrows.
    
    // Logical elements
    Clothes_SLD* clothes_menu;      ///< Clothes selection menu.
    Perso_SDL* perso_menu_man;      ///< Male character in menu.
    Perso_SDL* perso_menu_woman;    ///< Female character in menu.
    struct Game* game;              ///< Reference to the game.
    int woman;                      ///< Flag indicating if the character is female.
} GameMenu;

/**
 * @brief Creates a new GameMenu object.
 * @param renderer The SDL renderer.
 * @return A pointer to the newly created GameMenu object.
 */
GameMenu* GameMenu_new(SDL_Renderer* renderer);

/**
 * @brief Sets the game instance for the game menu.
 * @param menu The GameMenu object.
 * @param game The game instance.
 */
void GameMenu_setGame(GameMenu* menu, struct Game* game);

/**
 * @brief Dispatches events to the game menu.
 * @param menu The GameMenu object.
 * @param event The SDL event.
 */
void GameMenu_dispatchEvents(GameMenu* menu, SDL_Event* event);

/**
 * @brief Draws the game menu.
 * @param menu The GameMenu object.
 */
void GameMenu_draw(GameMenu* menu);

/**
 * @brief Deletes the GameMenu object and frees its resources.
 * @param menu The GameMenu object to delete.
 */
void GameMenu_delete(GameMenu* menu);

/**
 * Initialise le menu de personnalisation comprenant des vêtements, un personnage et des flèches de navigation.
 * 
 * @param clothes_menu Pointeur vers le pointeur de la structure Clothes_SLD pour stocker le menu de vêtements initialisé.
 * @param perso_menu Pointeur vers le pointeur de la structure Perso_SDL pour stocker le personnage initialisé dans le menu.
 * @param arrows_menu Pointeur vers un tableau de pointeurs WDG_Button pour stocker les flèches de navigation initialisées.
 * @param renderer SDL_Renderer pour le rendu des textures.
 */
void init_menu(Clothes_SLD** clothes_menu, Perso_SDL** perso_menu_man, Perso_SDL** perso_menu_woman, WDG_Button*** arrows_menu, SDL_Renderer* renderer);

/**
 * Initialise le menu de sélection de vêtements pour la personnalisation du personnage.
 * 
 * @param renderer SDL_Renderer pour le rendu des textures.
 * @return Un pointeur vers la structure Clothes_SLD initialisée, ou NULL en cas d'échec.
 */
Clothes_SLD* init_clothes_menu(SDL_Renderer* renderer);

/**
 * Initialise des flèches directionnelles pour le menu de sélection de vêtements.
 * 
 * @param clothes Pointeur vers la structure Clothes_SLD gérant le menu.
 * @return Un tableau de pointeurs WDG_Button pour les flèches, ou NULL en cas d'échec.
 */
WDG_Button** init_arrows_menu(Clothes_SLD* clothes);

/**
 * Change l'élément vestimentaire affiché dans le menu de sélection de vêtements.
 * 
 * @param clothes Pointeur vers la structure Clothes_SLD gérant le menu.
 * @param direction Direction du changement (-1 pour gauche, 1 pour droite).
 * @param item Indice de l'élément vestimentaire à modifier.
 */
void changeClothes(Clothes_SLD* clothes, int direction, int item);

void changeHairLeft(WDG_Button* button, void* clothes);
void changeEyesLeft(WDG_Button* button, void* clothes);
void changeTopLeft(WDG_Button* button, void* clothes);
void changeBottomLeft(WDG_Button* button, void* clothes);
void changeHairRight(WDG_Button* button, void* clothes);
void changeEyesRight(WDG_Button* button, void* clothes);
void changeTopRight(WDG_Button* button, void* clothes);
void changeBottomRight(WDG_Button* button, void* clothes); 


/**
 * Libère la mémoire allouée pour un menu de sélection de vêtements.
 * 
 * @param clothes Pointeur vers la structure Clothes_SLD représentant le menu.
 */
void free_clothes_menu(Clothes_SLD* clothes);

/**
 * Libère la mémoire allouée pour les flèches directionnelles dans le menu de sélection de vêtements.
 * 
 * @param arrows Tableau de pointeurs WDG_Button représentant les flèches directionnelles.
 */
void free_arrows(WDG_Button** arrows);

#endif