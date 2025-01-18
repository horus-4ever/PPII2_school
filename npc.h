#ifndef NPC_H
#define NPC_H

#include <stdbool.h>
#include "perso_SDL.h"
#include "quest/quest.h"

// Représente un point avec des coordonnées x et y
typedef struct {
    int x, y;
} Point;

// Représente un élément de la file d'attente pour l'algorithme de recherche de chemin
typedef struct {
    Point pt;      // Coordonnées du point actuel
    int length;    // Longueur du chemin jusqu'à ce point
    char* path;    // Chemin parcouru sous forme de chaîne de caractères
} QueueElement;

// Indique l'état de déplacement d'un NPC, ainsi que la direction du prochain mouvement
typedef struct {
    int up, down, left, right;  // Indicateurs de possibilité de mouvement dans chaque direction
    char nextDirection;         // Direction du prochain mouvement
} MoveState;

/**
 * Vérifie si les coordonnées (x, y) sont valides dans les matrices de fond et d'objets.
 *
 * @param x Coordonnée x.
 * @param y Coordonnée y.
 * @param background Matrice représentant le fond.
 * @param objects Matrice représentant les objets.
 * @param rows Nombre de lignes dans les matrices.
 * @param cols Nombre de colonnes dans les matrices.
 * @return true si les coordonnées sont valides, sinon false.
 */
bool isValid(int x, int y, int** background, int** objects, int rows, int cols);

/**
 * Trouve le chemin le plus court entre deux points dans les matrices de fond et d'objets.
 *
 * @param background Matrice représentant le fond.
 * @param objects Matrice représentant les objets.
 * @param rows Nombre de lignes dans les matrices.
 * @param cols Nombre de colonnes dans les matrices.
 * @param startx Coordonnée x de départ.
 * @param starty Coordonnée y de départ.
 * @param destx Coordonnée x de destination.
 * @param desty Coordonnée y de destination.
 * @param returnSize Pointeur vers la taille du chemin retourné.
 * @return Tableau de caractères représentant le chemin le plus court.
 */
char** closestPath(int** background, int** objects, int rows, int cols, int startx, int starty, int destx, int desty, int* returnSize);

/**
 * Initialise un état de mouvement pour un NPC.
 *
 * @return Pointeur vers une nouvelle instance de MoveState.
 */
MoveState* init_moveState();

/**
 * Détermine le prochain mouvement d'un NPC.
 *
 * @param moveState État de mouvement actuel du NPC.
 * @return Pointeur vers l'état de mouvement mis à jour.
 */
MoveState* nextMove(MoveState* moveState);

// Représente une liste chaînée de personnages (NPCs)
typedef struct NPCList {
    Perso_SDL* perso;       // Pointeur vers un personnage SDL
    struct NPCList* next;   // Pointeur vers le prochain élément de la liste
} NPCList;

/**
 * Initialise une liste chaînée de NPCs.
 *
 * @return Pointeur vers une nouvelle instance de NPCList.
 */
NPCList* init_list();

/**
 * Ajoute un personnage à la liste chaînée de NPCs.
 *
 * @param head Pointeur vers la tête de la liste.
 * @param newPerso Pointeur vers le nouveau personnage à ajouter.
 */
void add_perso(NPCList** head, Perso_SDL* newPerso);

/**
 * Supprime un personnage de la liste chaînée de NPCs en fonction de ses coordonnées.
 *
 * @param head Pointeur vers la tête de la liste.
 * @param x Coordonnée x du personnage à supprimer.
 * @param y Coordonnée y du personnage à supprimer.
 */
void remove_perso(NPCList** head, int x, int y);

/**
 * Libère la mémoire allouée pour la liste chaînée de NPCs.
 *
 * @param head Pointeur vers la tête de la liste.
 */
void free_list(NPCList* head);

/**
 * Met à jour la position de tous les NPCs dans une liste.
 *
 * @param npcs Pointeur vers la liste des NPCs.
 * @param x Nouvelle coordonnée x.
 * @param y Nouvelle coordonnée y.
 */
void update_position_all_npc(ArrayList* npcs, int x, int y);

/**
 * Met à jour la position d'un NPC spécifique.
 *
 * @param npc Pointeur vers le NPC.
 * @param x Nouvelle coordonnée x.
 * @param y Nouvelle coordonnée y.
 */
void update_position_specific_npc(Perso_SDL* npc, int x, int y);

/**
 * Affiche tous les NPCs à l'écran en utilisant SDL.
 *
 * @param npcs Pointeur vers la liste des NPCs.
 * @param renderer Pointeur vers le renderer SDL.
 */
void NPC_SDL_RenderCopy(ArrayList* npcs, SDL_Renderer* renderer, Perso_SDL** clothes, Perso_SDL* perso);

/**
 * Réinitialise l'animation de tous les NPCs dans une liste.
 *
 * @param npcs Pointeur vers la liste des NPCs.
 */
void reset_animation_npc(ArrayList* npcs);

/**
 * Fonction du thread pour déplacer les NPCs de manière continue et automatique.
 *
 * @param arg Pointeur vers les arguments du thread.
 * @return Pointeur vers le résultat du thread (NULL dans ce cas).
 */
void* thread_function(void* arg);

// Arguments pour le thread de déplacement automatique des NPCs
typedef struct {
    MoveState* moveState;  // Pointeur vers l'état de déplacement des NPCs
    ArrayList* npcs;       // Pointeur vers la liste des NPCs
    Perso_SDL* player;     // Pointeur vers le joueur
} ThreadArgs;

#endif