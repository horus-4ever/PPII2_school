//
// Created by marc on 09/05/24.
//

#include "sound.h"

#include <stdio.h>

/**
 * Initialise la partie audio de la SDL
 */
void init_audio(){
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
}

/**
 * Initialise le tableau avec le même bruit de pas en deux fois pour faire un bruit de pas réaliste
 * @return Le tableau
 */
Mix_Chunk** init_footstep(){
    Mix_Chunk** footsteps = (Mix_Chunk**)malloc(sizeof(Mix_Chunk*)*NB_FOOTSTEP);
    footsteps[0] = load_sound("ressources/sound/footstep.ogg");
    footsteps[1] = load_sound("ressources/sound/footstep.ogg");
    return footsteps;
}

/**
 * Joue le bruit de pas
 * @param pas Le tableau de bruits de pas
 * @param current_footstep Compteur de bruit de pas
 * @return Le compteur de bruit de pas mis à jour
 */
int play_footstep(Mix_Chunk** pas, int current_footstep){
    Mix_PlayChannel(-1,pas[current_footstep],0);
    current_footstep = (current_footstep + 1)%NB_FOOTSTEP;
    return current_footstep;
}

/**
 * Charge la musique à partir du fichier associé
 * @param path Le chemin vers le fichier que l'on veut charger
 * @return La musique
 */
Mix_Music* load_music(const char path[]){
    FILE* f = fopen(path,"r");
    if(f != NULL){
        fclose(f);
        Mix_Music* mus = Mix_LoadMUS(path);
        if(mus == NULL){
            fprintf(stderr, "Erreur: impossible de charger la musique %s.\n",path);
        }
        return mus;
    }
    else{
        fprintf(stderr, "Erreur: le fichier %s n'existe pas.\n",path);
        return NULL;
    }
}

/**
 * Charge le son à partir du fichier son associé
 * @param path  Le chemin vers le fichier que l'on veut charger
 * @return Le son
 */
Mix_Chunk* load_sound(const char path[]){
    FILE* f = fopen(path,"r");
    if(f != NULL){
        fclose(f);
        Mix_Chunk* sound = Mix_LoadWAV(path);
        if (sound == NULL){
            fprintf(stderr, "Erreur: impossible de charger le son %s.\n",path);
        }
        return sound;
    }
    else{
        fprintf(stderr, "Erreur: le fichier %s n'existe pas.\n",path);
        return NULL;
    }
}

/**
 * Initialise les différents sons utilisés
 * @return La structure Sound qui contient tout les sons utilisés dans le jeu
 */
Mix_Chunk** init_sounds(){
    Mix_Chunk** sons = (Mix_Chunk**)malloc(sizeof(Mix_Chunk*)*NB_SOUND);
    sons[INDEX_PORTE] = load_sound("ressources/sound/door.ogg");
    sons[INDEX_ITEM_RECUP] = load_sound("ressources/sound/item.wav");
    sons[INDEX_MENU] = load_sound("ressources/sound/menu.wav");
    return sons;
}

/**
 * Joue le son demandé
 * @param sons Le tableau de sons
 * @param index L'indice correspondant au son demandé
 */
void play_sound(Mix_Chunk** sons, int index){
    Mix_PlayChannel(-1, sons[index], 0);
}

/**
 * Ajute le volume de la musique
 * @param volume Le nouveau volume
 */
void change_volume_music(int volume){
    // Ajuste le volume pour qu'il se trouve dans l'intervale de volume accepté si nécessaire
    volume = (volume < 0) ? 0 : volume;
    volume = (volume > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume;
    Mix_VolumeMusic(volume);
}

/**
 * Ajuste le volume des sons
 * @param sons Tableau de sons dont on veut modifier le volume
 * @param volume Le nouveau volume
 */
void change_volume_sons(Mix_Chunk** sons,int volume){
    // Ajuste le volume pour qu'il se trouve dans l'intervale de volume accepté si nécessaire
    volume = (volume < 0) ? 0 : volume;
    volume = (volume > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume;
    for(int i=0;i<NB_SOUND;i++){
        Mix_VolumeChunk(sons[i],volume);
    }
}

/**
 * Ajuste le volume des sons
 * @param sons Tableau de bruit de pas dont on veut modifier le volume
 * @param volume Le nouveau volume
 */
void change_volume_footstep(Mix_Chunk** pas,int volume){
    // Ajuste le volume pour qu'il se trouve dans l'intervale de volume accepté si nécessaire
    volume = (volume < 0) ? 0 : volume;
    volume = (volume > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume;
    for(int i=0;i<NB_FOOTSTEP;i++){
        Mix_VolumeChunk(pas[i],volume);
    }
}

Cursor* init_cursor(int i, SDL_Renderer* renderer){
    Cursor* new = malloc(sizeof(Cursor));
    new->srcBar = (SDL_Rect){0, 0, 170, 15};
    new->srcCursor = (SDL_Rect){0, 0, 17, 30};
    new->dstBar = (SDL_Rect){150, 125+42*(i+1), 170, 15};
    new->dstCursor = (SDL_Rect){MAX_CURSOR-50, 125+42*(i+1) -7, 17, 30};
    new->spriteBar = load_image("ressources/images/cursor_bar.png", renderer);
    new->spriteCursor = load_image("ressources/images/cursor.png", renderer);
    return new;
}

void render_cursor(Cursor* cursor, SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, cursor->spriteBar, &(cursor->srcBar), &(cursor->dstBar));
    SDL_RenderCopy(renderer, cursor->spriteCursor, &(cursor->srcCursor), &(cursor->dstCursor));
}

int calcul_volume(int x_cursor){
    int volume = ((x_cursor - MIN_CURSOR) * MIX_MAX_VOLUME)/(MAX_CURSOR - MIN_CURSOR);
    return volume;
}

void clickCursor(Cursor* cursor, int mouseX, int* isDraggingCursor, int num, Mix_Chunk** s){
    if (mouseX >= cursor->dstCursor.x && mouseX <= cursor->dstCursor.x +30){
        *isDraggingCursor = num;
    }
    else if (mouseX >= MIN_CURSOR && mouseX <= MAX_CURSOR+17){
        cursor->dstCursor.x = (mouseX <= MIN_CURSOR+8) ? MIN_CURSOR : (mouseX >= MAX_CURSOR) ? MAX_CURSOR : mouseX-8;
        switch (num)
        {
        case 1:
            change_volume_music(calcul_volume(mouseX-8));
            break;
        case 2:
            change_volume_footstep(s, calcul_volume(mouseX-8));
            break;
        case 3:
            change_volume_sons(s, calcul_volume(mouseX-8));
            break;
        default:
            break;
        }
    }
}


/**
 * Libère les sons en mémoire
 * @param sons Les sons que l'on veut libérer de la mémoire
 */
void free_sons(Mix_Chunk** sons){
    for(int i=0;i<NB_SOUND;i++){
        Mix_FreeChunk(sons[i]);
    }
    free(sons);
}

/**
 * Libère les sons de pas en mémoire
 * @param pas Le tableau de sons de pas que l'on veut libérer de la mémoire
 */
void free_pas(Mix_Chunk** pas){
    for(int i=0;i<NB_FOOTSTEP;i++){
        Mix_FreeChunk(pas[i]);
    }
    free(pas);
}

void free_cursor(Cursor* cursor){
    if (cursor == NULL) return;
    SDL_DestroyTexture(cursor->spriteBar);
    SDL_DestroyTexture(cursor->spriteCursor);
    free(cursor);
}


/**
 * ==================== MANAGER ====================
 * =================================================
*/

SoundManager* SoundManager_new(void) {
    Mix_Music* music = load_music("ressources/sound/musique.mp3");
    if(music == NULL){
        return NULL;
    }
    Mix_PlayMusic(music,-1);
    Mix_Chunk** sons = init_sounds();
    if(sons == NULL){
        return NULL;
    }
    Mix_Chunk** pas = init_footstep();
    if(pas == NULL){
        return NULL;
    }
    // construct the object
    SoundManager* result = malloc(sizeof(SoundManager));
    result->footstep = pas;
    result->sounds = sons;
    result->background = music;
    result->sound = 1; // there is sound (no mute)
    return result;
}

void SoundManager_playMusic(SoundManager* manager, Mix_Chunk* music) {
    Mix_PlayChannel(-1, music, 0);
}

void SoundManager_setGame(SoundManager* manager, struct Game* game) {
    manager->game = game;
}

void SoundManager_delete(SoundManager* manager) {
    free_pas(manager->footstep);
    free_sons(manager->sounds);
    Mix_FreeMusic(manager->background);
    // free the pointer
    free(manager);
}