//
// Created by marc on 09/05/24.
//

#ifndef VIDEO_GAME_07_SOUND_H
#define VIDEO_GAME_07_SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "fonctions_SDL.h"

#define NB_SOUND 3
#define INDEX_PORTE 0
#define INDEX_ITEM_RECUP 1
#define INDEX_MENU 2

#define NB_FOOTSTEP 2
#define FOOTSTEP_DELAY 400

#define MIN_CURSOR 153
#define MAX_CURSOR 300

typedef struct {
    SDL_Rect srcBar; 
    SDL_Rect dstBar;
    SDL_Texture* spriteBar; 
    SDL_Rect srcCursor; 
    SDL_Rect dstCursor;
    SDL_Texture* spriteCursor;
} Cursor;

typedef struct {
    int* parameters;
    Mix_Chunk** sons;
} ToggleParams;

typedef struct {
    int* sound;
    Mix_Chunk** sons;
    Mix_Chunk ** pas;
    Cursor* music;
    Cursor* sounds;
    Cursor* footsteps;
} ToggleSoundParams;

struct Game;

typedef struct SoundManager {
    Mix_Chunk** footstep;
    Mix_Chunk** sounds;
    Mix_Music* background;
    struct Game* game;
    int sound;
} SoundManager;

SoundManager* SoundManager_new(void);
void SoundManager_setGame(SoundManager* manager, struct Game* game);
void SoundManager_delete(SoundManager* manager);
void SoundManager_playMusic(SoundManager* manager, Mix_Chunk* music);


/**
 * Initialise la partie audio de la SDL
 */
void init_audio();

/**
 * Initialise le tableau avec le même bruit de pas en deux fois pour faire un bruit de pas réaliste
 * @return Le tableau
 */
Mix_Chunk** init_footstep();

/**
 * Joue le bruit de pas
 * @param pas Le tableau de bruits de pas
 * @param current_footstep Compteur de bruit de pas
 * @return Le compteur de bruit de pas mis à jour
 */
int play_footstep(Mix_Chunk** pas, int current_footstep);

/**
 * Charge la musique à partir du fichier associé
 * @param path Le chemin vers le fichier que l'on veut charger
 * @return La musique
 */
Mix_Music* load_music(const char path[]);

/**
 * Charge le son à partir du fichier son associé
 * @param path  Le chemin vers le fichier que l'on veut charger
 * @return Le son
 */
Mix_Chunk* load_sound(const char path[]);

/**
 * Initialise les différents sons utilisés
 * @return un tableau qui contient tout les sons utilisés dans le jeu
 */
Mix_Chunk** init_sounds();

/**
 * Joue le son demandé
 * @param sons Le tableau de sons
 * @param index L'indice correspondant au son demandé
 */
void play_sound(Mix_Chunk** sons, int index);

/**
 * Ajuste le volume de la musique
 * @param volume Le nouveau volume
 */
void change_volume_music(int volume);

/**
 * Ajuste le volume des sons
 * @param sons Tableau de sons dont on veut modifier le volume
 * @param volume Le nouveau volume
 */
void change_volume_sons(Mix_Chunk** sons,int volume);

/**
 * Ajuste le volume des sons
 * @param sons Tableau de bruit de pas dont on veut modifier le volume
 * @param volume Le nouveau volume
 */
void change_volume_footstep(Mix_Chunk** pas,int volume);

Cursor* init_cursor(int i, SDL_Renderer* renderer);

void render_cursor(Cursor* cursor, SDL_Renderer* renderer);

int calcul_volume(int x_cursor);

void clickCursor(Cursor* cursor, int mouseX, int* isDraggingCursor, int num, Mix_Chunk** s);

/**
 * Libère les sons en mémoire
 * @param sons Les sons que l'on veut libérer de la mémoire
 */
void free_sons(Mix_Chunk** sons);

/**
 * Libère les sons de pas en mémoire
 * @param p Le tableau de sons de pas que l'on veut libérer de la mémoire
 */
void free_pas(Mix_Chunk** pas);

void free_cursor(Cursor* cursor);

#endif //VIDEO_GAME_07_SOUND_H
