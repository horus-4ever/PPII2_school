#include "perso_SDL.h"
#include "map_SDL.h"
#include "map.h"
#include "widgets/widgets.h"
#include <SDL2/SDL.h>
#include "menu.h"
#include "game.h"

/**
 * Initialise le menu de personnalisation comprenant des vêtements, un personnage et des flèches de navigation.
 * 
 * @param clothes_menu Pointeur vers le pointeur de la structure Clothes_SLD pour stocker le menu de vêtements initialisé.
 * @param perso_menu Pointeur vers le pointeur de la structure Perso_SDL pour stocker le personnage initialisé dans le menu.
 * @param arrows_menu Pointeur vers un tableau de pointeurs WDG_Button pour stocker les flèches de navigation initialisées.
 * @param renderer SDL_Renderer pour le rendu des textures.
 */
void init_menu(Clothes_SLD** clothes_menu, Perso_SDL** perso_menu_man, Perso_SDL** perso_menu_woman, WDG_Button*** arrows_menu, SDL_Renderer* renderer){
    *clothes_menu = init_clothes_menu(renderer);
    *perso_menu_man = init_perso(WIDTH/2 - SIZE_PERSO_MENU/2 + OFFSET_PERSO_X + OFFSET_MAP_X,HEIGHT/2 - SIZE_PERSO_MENU/2 + OFFSET_PERSO_Y + OFFSET_MAP_Y,SIZE_PERSO_MENU,SIZE_PERSO_MENU, 3, "ressources/images/vetements/man/man.png",renderer,8,5);
    *perso_menu_woman = init_perso(WIDTH/2 - SIZE_PERSO_MENU/2 + OFFSET_PERSO_X + OFFSET_MAP_X,HEIGHT/2 - SIZE_PERSO_MENU/2 + OFFSET_PERSO_Y + OFFSET_MAP_Y,SIZE_PERSO_MENU,SIZE_PERSO_MENU, 3, "ressources/images/vetements/woman/woman.png",renderer,8,5);
    *arrows_menu = init_arrows_menu(*clothes_menu);
}

/**
 * Initialise le menu de sélection de vêtements pour la personnalisation du personnage.
 * 
 * @param renderer SDL_Renderer pour le rendu des textures.
 * @return Un pointeur vers la structure Clothes_SLD initialisée, ou NULL en cas d'échec.
 */
Clothes_SLD* init_clothes_menu(SDL_Renderer* renderer){
    Clothes_SLD* result = malloc(sizeof(Clothes_SLD));
    if (result == NULL){
        return NULL;
    }

    int* cheveux = malloc(sizeof(int));
    int* oeil = malloc(sizeof(int));
    int* haut = malloc(sizeof(int));
    int* pantalon = malloc(sizeof(int));
    *cheveux = rand() %3;
    *oeil = rand() %3;
    *haut = rand() %3;
    *pantalon = rand() %3;

    Perso_SDL** clothes_menu_woman = malloc(4*sizeof(Perso_SDL));
    Perso_SDL** clothes_menu_man = malloc(4*sizeof(Perso_SDL));
    if (clothes_menu_woman == NULL || clothes_menu_man == NULL){
        return NULL;
    }
    int pos_x = WIDTH/2 - SIZE_PERSO_MENU/2 + OFFSET_PERSO_X + OFFSET_MAP_X;
    int pos_y = HEIGHT/2 - SIZE_PERSO_MENU/2 + OFFSET_PERSO_Y + OFFSET_MAP_Y;
    //cheveux
    clothes_menu_woman[0] = init_perso(pos_x,pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3, "ressources/images/vetements/woman/c.png",renderer,3,1);
    clothes_menu_man[0] = init_perso(pos_x,pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3, "ressources/images/vetements/man/c.png",renderer,3,1);
    //yeux
    clothes_menu_woman[1] = init_perso(pos_x,pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3, "ressources/images/vetements/woman/o.png",renderer,3,1);
    clothes_menu_man[1] = init_perso(pos_x,pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3,"ressources/images/vetements/man/o.png",renderer,3,1);
    //top
    clothes_menu_woman[2] = init_perso(pos_x, pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3,"ressources/images/vetements/woman/h.png",renderer,3,1);
    clothes_menu_man[2] = init_perso(pos_x, pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3,"ressources/images/vetements/man/h.png",renderer,3,1);
    //pants
    clothes_menu_woman[3] = init_perso(pos_x,pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3,"ressources/images/vetements/woman/p.png",renderer,3,1);
    clothes_menu_man[3] = init_perso(pos_x,pos_y,SIZE_PERSO_MENU,SIZE_PERSO_MENU,3,"ressources/images/vetements/man/p.png",renderer,3,1);
    

    for (int i = 0 ; i<4 ; i++){
        if (clothes_menu_woman[i] == NULL || clothes_menu_man == NULL){
            return NULL;
        }
    }
    clothes_menu_man[0]->srcRect.x = 32* (*cheveux);
    clothes_menu_man[1]->srcRect.x = 32* (*oeil);
    clothes_menu_man[2]->srcRect.x = 32* (*haut);
    clothes_menu_man[3]->srcRect.x = 32* (*pantalon);
    clothes_menu_woman[0]->srcRect.x = 32* (*cheveux);
    clothes_menu_woman[1]->srcRect.x = 32* (*oeil);
    clothes_menu_woman[2]->srcRect.x = 32* (*haut);
    clothes_menu_woman[3]->srcRect.x = 32* (*pantalon);

    result->clothes_menu_man = clothes_menu_man;
    result->clothes_menu_woman = clothes_menu_woman;
    result->cheveux = cheveux;
    result->oeil = oeil;
    result->pantalon = pantalon;
    result->haut = haut;

    return result;
}

/**
 * Initialise des flèches directionnelles pour le menu de sélection de vêtements.
 * 
 * @param clothes Pointeur vers la structure Clothes_SLD gérant le menu.
 * @return Un tableau de pointeurs WDG_Button pour les flèches, ou NULL en cas d'échec.
 */
WDG_Button** init_arrows_menu(Clothes_SLD* clothes){
    WDG_Button** result = malloc(8*sizeof(WDG_Button*));
    if (result == NULL){
        return NULL;
    }
    for (int i = 0; i<4;i++){
        result[i] = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/left_arrow.png")), 64, 64);
        if (result[i] == NULL){
            return NULL;
        }
        WDG_buttonSetPos(result[i], WIDTH/2 - SIZE_PERSO_MENU/2 - 50, HEIGHT/2 - SIZE_PERSO_MENU/2 + SIZE_PERSO_MENU/6*(i) + SIZE_PERSO_MENU/5);
    }
    WDG_buttonSetOnClick(result[0], changeHairLeft, clothes);
    WDG_buttonSetOnClick(result[1], changeEyesLeft, clothes);
    WDG_buttonSetOnClick(result[2], changeTopLeft, clothes);
    WDG_buttonSetOnClick(result[3], changeBottomLeft, clothes);

    for (int i = 4; i<8;i++){
        result[i] = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/right_arrow.png")), 64, 64);
        if (result[i] == NULL){
            return NULL;
        }
        WDG_buttonSetPos(result[i], WIDTH/2 + SIZE_PERSO_MENU/2 + 50-64, HEIGHT/2 - SIZE_PERSO_MENU/2 + SIZE_PERSO_MENU/6*(i-4) + SIZE_PERSO_MENU/5);
    }
    WDG_buttonSetOnClick(result[4], changeHairRight, clothes);
    WDG_buttonSetOnClick(result[5], changeEyesRight, clothes);
    WDG_buttonSetOnClick(result[6], changeTopRight, clothes);
    WDG_buttonSetOnClick(result[7], changeBottomRight, clothes);
    return result;
}

/**
 * Change l'élément vestimentaire affiché dans le menu de sélection de vêtements.
 * 
 * @param clothes Pointeur vers la structure Clothes_SLD gérant le menu.
 * @param direction Direction du changement (-1 pour gauche, 1 pour droite).
 * @param item Indice de l'élément vestimentaire à modifier.
 */
void changeClothes(Clothes_SLD* clothes, int direction, int item){
    int w = get_width_spritesheet(clothes->clothes_menu_woman[item]);
    clothes->clothes_menu_man [item]->srcRect.x += direction*(w/3);
    clothes->clothes_menu_man[item]->srcRect.x += w;
    clothes->clothes_menu_man[item]->srcRect.x %= w;
    clothes->clothes_menu_woman[item]->srcRect.x += direction*(w/3);
    clothes->clothes_menu_woman[item]->srcRect.x += w;
    clothes->clothes_menu_woman[item]->srcRect.x %= w;

    switch (item)
    {
    case 0:
        *(clothes->cheveux) += direction + 3;
        *(clothes->cheveux) %= 3;
        break;
    case 1:
        *(clothes->oeil) += direction + 3;
        *(clothes->oeil) %= 3;
        break;
    case 2:
        *(clothes->haut) += direction + 3;
        *(clothes->haut) %= 3;
        break;
    case 3:
        *(clothes->pantalon) += direction + 3;
        *(clothes->pantalon) %= 3;
        break;
    default:
        break;
    }

}

void changeHairLeft(WDG_Button* button, void* clothes){changeClothes((Clothes_SLD*)clothes, -1, 0);}
void changeEyesLeft(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, -1, 1);}
void changeTopLeft(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, -1, 2);}
void changeBottomLeft(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, -1, 3);}
void changeHairRight(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, 1, 0);}
void changeEyesRight(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, 1, 1);}
void changeTopRight(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, 1, 2);}
void changeBottomRight(WDG_Button* button, void* clothes) {changeClothes((Clothes_SLD*)clothes, 1, 3);}

/**
 * Libère la mémoire allouée pour un menu de sélection de vêtements.
 * 
 * @param clothes Pointeur vers la structure Clothes_SLD représentant le menu.
 */
void free_clothes_menu(Clothes_SLD* clothes){
    free(clothes->cheveux);
    free(clothes->oeil);
    free(clothes->haut);
    free(clothes->pantalon);
    for (int i = 0; i<4;i++){
        free_perso(clothes->clothes_menu_man[i]);
        free_perso(clothes->clothes_menu_woman[i]);
    }
    free(clothes->clothes_menu_woman);
    free(clothes->clothes_menu_man);
    free(clothes);
}

/**
 * Libère la mémoire allouée pour les flèches directionnelles dans le menu de sélection de vêtements.
 * 
 * @param arrows Tableau de pointeurs WDG_Button représentant les flèches directionnelles.
 */
void free_arrows(WDG_Button** arrows){
    for (int i=0;i<8;i++){
        WDG_deleteButton(arrows[i]);
    }
    free(arrows);
}

/**
 * ==============================================================
 * ========================= GAME MENU ==========================
 * ==============================================================
*/

void changeToMan(WDG_Button* button, void* boolean) {
    int woman = *(int*)boolean ;
    if (woman){
       *(int*)boolean = 0;
    }   
}

void changeToWoman(WDG_Button* button, void* boolean) {
    int woman = *(int*)boolean ;
    if (!woman){
       *(int*)boolean = 1;
    }   
}

void onSave(WDG_Button* button, void* params) {
    // get the param
    GameMenu* menu = params;
    Game* game = menu->game;
    SoundManager* soundManager = game->soundManager;
    // spawn
    int spawn_x = WIDTH/2 - SIZE_PERSO/2 + OFFSET_PERSO_X + OFFSET_MAP_X;
    int spawn_y = HEIGHT/2 - SIZE_PERSO/2 + OFFSET_PERSO_Y + OFFSET_MAP_Y;
    // perform the computations
    reset_animation_perso(game->perso);
    game->perso->srcRect.y = 0;
    init_clothes_perso(&game->clothes_perso, game->renderer, *(menu->clothes_menu->cheveux), *(menu->clothes_menu->oeil), *(menu->clothes_menu->haut), *(menu->clothes_menu->pantalon), spawn_x, spawn_y, menu->woman);
    play_sound(soundManager->sounds,INDEX_MENU);
    if (menu->woman){
        SDL_DestroyTexture(game->perso->sprite);
        game->perso->sprite = load_image("ressources/images/vetements/woman/woman.png", game->renderer);
    } else {
        SDL_DestroyTexture(game->perso->sprite);
        game->perso->sprite = load_image("ressources/images/vetements/man/man.png", game->renderer);
    }
    // set the new game state
    Game_setState(game, GAME_PLAYING);
}


GameMenu* GameMenu_new(SDL_Renderer* renderer) {
    GameMenu* result = malloc(sizeof(GameMenu));
    // creation of buttons and other graphical element
    // save button
    WDG_Button* buttonSave = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/button_save.png")), 243, 85);
    WDG_buttonSetPos(buttonSave, WIDTH/2 - 243/2, 720);
    WDG_buttonSetOnClick(buttonSave, onSave, result);
    // select the man button
    WDG_Button* buttonMan = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/button_man.png")), 85, 85);
    WDG_buttonSetPos(buttonMan, WIDTH - 85*2 - 30*2, 30);
    WDG_buttonSetOnClick(buttonMan, changeToMan, &result->woman);
    // select the woman button
    WDG_Button* buttonWoman = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/button_woman.png")), 85, 85);
    WDG_buttonSetPos(buttonWoman, WIDTH - 85 - 30, 30);
    WDG_buttonSetOnClick(buttonWoman, changeToWoman, &result->woman);
    // background
    SDL_Texture* background = load_image("ressources/images/background2.png", renderer);
    if (!background) {
        printf("Erreur lors de la création du background 2\n");fflush(stdout);
        return NULL;
    }
    // clothes
    Clothes_SLD* clothes_menu = NULL;
    Perso_SDL* perso_menu_man = NULL;
    Perso_SDL* perso_menu_woman = NULL;
    WDG_Button** arrows_menu = NULL;
    init_menu(&clothes_menu, &perso_menu_man, &perso_menu_woman, &arrows_menu, renderer);
    /* Construction of the menu */
    result->buttonSave = buttonSave;
    result->buttonMan = buttonMan;
    result->buttonWoman = buttonWoman;
    result->background = background;
    result->clothes_menu = clothes_menu;
    result->perso_menu_man = perso_menu_man;
    result->perso_menu_woman = perso_menu_woman;
    result->arrows_menu = arrows_menu;
    result->woman = 1;
    return result;
}

void GameMenu_setGame(GameMenu* menu, Game* game) {
    menu->game = game;
}

void GameMenu_dispatchEvents(GameMenu* menu, SDL_Event* event) {
    WDG_buttonDispatchEvent(menu->buttonMan, event);
    WDG_buttonDispatchEvent(menu->buttonWoman, event);
    WDG_buttonDispatchEvent(menu->buttonSave, event);
    for (int i = 0 ; i < 8 ; i++){
        WDG_buttonDispatchEvent(menu->arrows_menu[i], event);
    }
    // specific events
    if(event->type == SDL_KEYDOWN){
        if(event->key.keysym.sym == SDLK_a ){
            Game_setState(menu->game, GAME_PLAYING);
        }
    }
}

void GameMenu_draw(GameMenu* menu) {
    // get the renderer
    SDL_Renderer* renderer = menu->game->renderer;
    // render the different things
    SDL_RenderCopy(renderer, menu->background, NULL, NULL);
    for(int i = 0 ; i < 8 ; i++){
        WDG_drawButton(menu->arrows_menu[i], renderer);
    }
    WDG_drawButton(menu->buttonSave, renderer);
    WDG_drawButton(menu->buttonMan, renderer);
    WDG_drawButton(menu->buttonWoman, renderer);
    WDG_TextArea* textarea = WDG_createTextArea(UTF8_copy(menu->game->player->name), WDG_getFont(DEFAULT_FONT, 40), WIDTH - 20, HEIGHT - 20);
    WDG_textareaSetFontColor(textarea, WDG_BLACK);
    WDG_textareaSetPos(textarea, 30, 40);
    WDG_drawTextArea(textarea, renderer);

    if (menu->woman)
    {
        SDL_RenderCopy(renderer, menu->perso_menu_woman->sprite, &menu->perso_menu_woman->srcRect, &menu->perso_menu_woman->dstRect);
        for (int i = 0; i < 4; i++)
        {
            SDL_RenderCopy(renderer, menu->clothes_menu->clothes_menu_woman[i]->sprite, &(menu->clothes_menu->clothes_menu_woman[i]->srcRect), &(menu->clothes_menu->clothes_menu_woman[i]->dstRect));
        }
    }
    else
    {
        SDL_RenderCopy(renderer, menu->perso_menu_man->sprite, &menu->perso_menu_man->srcRect, &menu->perso_menu_man->dstRect);
        for (int i = 0; i < 4; i++)
        {
            SDL_RenderCopy(renderer, menu->clothes_menu->clothes_menu_man[i]->sprite, &(menu->clothes_menu->clothes_menu_man[i]->srcRect), &(menu->clothes_menu->clothes_menu_man[i]->dstRect));
        }
    }
    WDG_deleteTextArea(textarea);
}

void GameMenu_delete(GameMenu* menu) {
    free_perso(menu->perso_menu_man);
    free_perso(menu->perso_menu_woman);
    free_clothes_menu(menu->clothes_menu);
    free_arrows(menu->arrows_menu);
    // graphical elements
    SDL_DestroyTexture(menu->background);
    WDG_deleteButton(menu->buttonMan);
    WDG_deleteButton(menu->buttonWoman);
    WDG_deleteButton(menu->buttonSave);
    // free the pointer
    free(menu);
}