//
// Created by marc on 15/03/24.
//

#ifndef VIDEO_GAME_07_MAP_H
#define VIDEO_GAME_07_MAP_H

#include <stdio.h>

/**
 * Structure représentant les données de la map du jeu
 */
struct _Map{
    int** m; // Matrice représentant la map du jeu
    int l; // Longueur de la matrice de la map
    int h; // Hauteur de la matrice de la map
};

typedef struct _Map Map;

/**
 * Récupère les informations dans le fichier texte donné censé décrire notre map
 * Il contiendra la largeur sur la première ligne
 * La hauteur sur la deuxième ligne
 * @param f Le fichier txt contenant les infos de notre map
 * @param l pointeur vers la variable qui contiendra la largeur
 * @param h pointeur vers la variable qui contiendra la hauteur
 * @return 0 si tout s'est bien passé, 1 si le chemin vers le fichier n'est pas bon et 2 si le fichier est mal formaté
 */
int get_info_map(FILE* f, int* l, int* h);

/**
 * Ajoute des bordures au jeu pour s'assurer que le monde n'affiche pas des carrés noirs lorsque le personnage
 * va vers les bordures du jeu. Pour cela on ajoute des bordures de "murs" tout autour de la map
 * @param l Le nombre de lignes que l'on va mettre à jour
 * @param h Le nombre de colonnes que l'on va mettre à jour
 */
void compute_matrix_size(int* l, int* h);

/**
 * Complete la map avec une bordures en mettant des zeros dans les cases qui forment la bordure
 * @param map La map
 * @param l Le nombre de lignes de la map en dehors des bordures
 * @param h Le nombre de colonnes de la map en dehors des bordures
 * @param type vaut 1 s'il s'agit de la matrice background, 0 si c'est la matrice object
 */
void complete_with_zeros(Map* map, int l, int h, int type);

/**
 * Lit la matrice contenue dans le fichier qui contient les infos du monde
 * @param f Le fichier qui contient les infos du monde
 * @param map La map
 * @param l Nombre de lignes hors bordure
 * @param h Nombre de colonnes hors bordure
 * @return 0 si tout s'est bien passé, 1 si le fichier est mal formaté
 */
int read_matrix(FILE* f, Map** map, int l, int h);

/**
 * Crée la map
 * @param l La largeur de la matrice qui représente la map
 * @param h La largeur de la matrice qui représente la map
 * @return La map fraîchement crée
 */
Map* create_map(int l, int h);

/**
 * Donne la longueur de la matrice dans la map
 * @param m La map
 * @return La longueur de la matrice dans la map
 */
int length_map(Map* m);

/**
 * Donne la hauteur de la matrice dans la map
 * @param m La map
 * @return La hauteur dans la map
 */
int height_map(Map* m);

/**
 * Donne la matrice dans la map
 * @param m La map
 * @return La matrice de la map
 */
int** matrix_map(Map* m);

/**
 * Vérfie que la taille de la matrice décrivant le monde soit suffisamment grande
 * pour être affichée sur la fenêtre de jeu sans afficher des blocs noirs
 * @param l La largeur de la matrice décrivant le monde
 * @param h La hauteur de la matrice décrivant le monde
 * @return 1 si la taille de la matrice est suffisamment grande, 0 sinon
 */
int check_length_matrix(int l, int h);

/**
 * Affiche la matrice de la map
 * @param map La map dont on veut afficher la matrice
 */
void print_map(Map* map);

/**
 * Fonction qui initialise entièrement la map à partir du fichier txt qui contient toutes les infos nécessaires
 * @param path nom/chemin vers le fichier txt qui contient toutes les infos sur notre map
 * @param map Pointeur vers la map que l'on va mettre à jour avec les données du fichier txt
 * @param type vaut 1 s'il s'agit de la map background, 0 si c'est la map object
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int init_map(char path[], Map** map, int type);

/**
 * Désalloue la map en mémoire
 * @param m La map que l'on veut libérer de la mémoire
 */
void free_map(Map* m);

void init_size_objects(char path[], int** object_l, int** object_h, int* nb_objects, int* nb_type_objects);

#endif //VIDEO_GAME_07_MAP_H
