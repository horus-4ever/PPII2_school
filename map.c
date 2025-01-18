#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "map_SDL.h"
#include "perso_SDL.h"

/**
 * Récupère les informations dans le fichier texte donné censé décrire notre map
 * Il contiendra la largeur sur la première ligne
 * La hauteur sur la deuxième ligne
 * @param f Le fichier txt contenant les infos de notre map
 * @param l pointeur vers la variable qui contiendra la largeur
 * @param h pointeur vers la variable qui contiendra la hauteur
 * @return 0 si tout s'est bien passé, 1 si le chemin vers le fichier n'est pas bon et 2 si le fichier est mal formaté
 */
int get_info_map(FILE* f, int* l, int* h){
    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier.\n");fflush(stdout);
        return 1;
    }
    if(fscanf(f,"%d\n%d\n",l,h) != 2) {
        printf("Erreur, impossible de lire le fichier.\n");
        fflush(stdout);
        return 2;
    }
    return 0;
}

/**
 * Ajoute des bordures au jeu pour s'assurer que le monde n'affiche pas des carrés noirs lorsque le personnage
 * va vers les bordures du jeu. Pour cela on ajoute des bordures de "murs" tout autour de la map
 * @param l Le nombre de lignes que l'on va mettre à jour
 * @param h Le nombre de colonnes que l'on va mettre à jour
 */
void compute_matrix_size(int* l, int* h){
    *l += 2*(HEIGHT/BLOC_SIZE);
    *h += 2*(WIDTH/BLOC_SIZE);
}

/**
 * Complete la map avec une bordures en mettant des zeros dans les cases qui forment la bordure
 * @param map La map
 * @param l Le nombre de lignes de la map en dehors des bordures
 * @param h Le nombre de colonnes de la map en dehors des bordures
 * @param type vaut 1 s'il s'agit de la matrice background, 0 si c'est la matrice object
 */
void complete_with_zeros(Map* map, int l, int h, int type){
    int val;
    if(type == 1){
        val = 4;
    }
    else{
        val = 0;
    }
    int** matrix = matrix_map(map);
    int deb_ligne = HEIGHT/BLOC_SIZE;
    int deb_colonne = WIDTH/BLOC_SIZE;
    // Partie haute de la bordure
    for(int i=0;i< deb_ligne;i++){
        for(int j=0;j<2*deb_colonne+h;j++){
            matrix[j][i] = val;
        }
    }
    // Partie gauche
    for(int i=deb_ligne;i<deb_ligne+l;i++){
        for(int j=0;j<deb_colonne;j++){
            matrix[j][i] = val;
        }
    }
    // Partie droite
    for(int i=deb_ligne;i<deb_ligne+l;i++){
        for(int j=deb_colonne+h;j<2*deb_colonne+h;j++){
            matrix[j][i] = val;
        }
    }
    // Partie basse
    for(int i=deb_ligne+l;i<2*deb_ligne+l;i++){
        for(int j=0;j<2*deb_colonne+h;j++){
            matrix[j][i] = val;
        }
    }
}

/**
 * Lit la matrice contenue dans le fichier qui contient les infos du monde
 * @param f Le fichier qui contient les infos du monde
 * @param map La map
 * @param l Nombre de lignes hors bordure
 * @param h Nombre de colonnes hors bordure
 * @return 0 si tout s'est bien passé, 1 si le fichier est mal formaté
 */
int read_matrix(FILE* f, Map** map, int l, int h){
    int** matrix = matrix_map(*map);
    int deb_ligne = HEIGHT/BLOC_SIZE;
    int deb_colonne = WIDTH/BLOC_SIZE;
    for(int i=deb_ligne;i<deb_ligne+l;i++){
        for(int j=deb_colonne;j<deb_colonne+h;j++){
            if(fscanf(f,"%d, ",&matrix[j][i]) != 1){
                printf("Le fichier est mal formaté.\n");fflush(stdout);
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Crée la map
 * @param l La largeur de la matrice qui représente la map
 * @param h La largeur de la matrice qui représente la map
 * @return La map fraîchement crée
 */
Map* create_map(int l, int h){
    Map* map = (Map*)malloc(sizeof(Map));
    map->l = l;
    map->h = h;
    int** m = (int**) malloc(h*sizeof(int*));
    for(int i=0;i<h;i++){
        m[i] = (int*) malloc(l*sizeof(int));
    }
    map->m = m;
    return map;
}

/**
 * Donne la longueur de la matrice dans la map
 * @param m La map
 * @return La longueur de la matrice dans la map
 */
int length_map(Map* m){
    return m->l;
}

/**
 * Donne la hauteur de la matrice dans la map
 * @param m La map
 * @return La hauteur dans la map
 */
int height_map(Map* m){
    return m->h;
}

/**
 * Donne la matrice dans la map
 * @param m La map
 * @return La matrice de la map
 */
int** matrix_map(Map* m){
    return m->m;
}

/**
 * Vérfie que la taille de la matrice décrivant le monde soit suffisamment grande
 * pour être affichée sur la fenêtre de jeu sans afficher des blocs noirs
 * @param l La largeur de la matrice décrivant le monde
 * @param h La hauteur de la matrice décrivant le monde
 * @return 1 si la taille de la matrice est suffisamment grande, 0 sinon
 */
int check_length_matrix(int l, int h){
    return (l*BLOC_SIZE >= HEIGHT && h*BLOC_SIZE >= WIDTH);
}

/**
 * Affiche la matrice de la map
 * @param map La map dont on veut afficher la matrice
 */
void print_map(Map* map){
    int** mat = matrix_map(map);
    for(int i=0;i< length_map(map);i++){
        for(int j=0;j< height_map(map);j++){
            printf("%d ",mat[j][i]);
        }
        printf("\n");
    }
}

/**
 * Fonction qui initialise entièrement la map à partir du fichier txt qui contient toutes les infos nécessaires
 * @param path nom/chemin vers le fichier txt qui contient toutes les infos sur notre map
 * @param map Pointeur vers la map que l'on va mettre à jour avec les données du fichier txt
 * @param type vaut 1 s'il s'agit de la map background, 0 si c'est la map object
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int init_map(char path[], Map** map, int type){
    FILE* f = fopen(path,"r");
    if(f == NULL){
        printf("Le nom/chemin vers le fichier est incorrect\n");fflush(stdout);
        return 1;
    }
    int l,h;
    if(get_info_map(f,&l,&h) != 0){
        printf("Info de la map non chargée.\n");fflush(stdout);
        return 1;
    }
    int actual_l = l, actual_h = h;
    compute_matrix_size(&l,&h);
    *map = create_map(l,h);
    if(*map == NULL){
        printf("La map n'a pas été crée correctement.\n");fflush(stdout);
        fclose(f);
        return 1;
    }
    if(!check_length_matrix(l,h)){
        printf("Le map n'est pas assez grande pour être affiché correctement.\n");fflush(stdout);
        return 1;
    }
    if(read_matrix(f,map,actual_l,actual_h)!=0){
        printf("type : %d\n", type);
        printf("Matrice de la map non chargée.\n");fflush(stdout);
        return 1;
    }
    complete_with_zeros(*map,actual_l,actual_h, type);
    fclose(f);
    return 0;
}

/**
 * Désalloue la map en mémoire
 * @param m La map que l'on veut libérer de la mémoire
 */
void free_map(Map* m){
    int** matrix = matrix_map(m);
    for(int i= height_map(m)-1;i>=0;i--){
        free(matrix[i]);
    }
    free(matrix);
    free(m);
}

void init_size_objects(char path[], int** objects_width, int** objects_height, int* nb_objects, int* nb_type_objects) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        printf("Le nom/chemin vers le fichier est incorrect\n");
        return;
    }

    fscanf(f, "%d\n", nb_objects);
    fscanf(f, "%d\n", nb_type_objects);

    *objects_width = malloc(sizeof(int) * (*nb_type_objects));
    *objects_height = malloc(sizeof(int) * (*nb_type_objects));

    if (*objects_width == NULL || *objects_height == NULL) {
        printf("Erreur, mémoire insuffisante pour les tableaux\n");
        fclose(f);
        return;
    }

    for (int i = 0; i < *nb_type_objects; i++) {
        int width, height;
        if (fscanf(f, "%d %d\n", &width, &height) != 2) {
            printf("Erreur de lecture des dimensions de l'objet %d\n", i+1);
            fclose(f);
            free(*objects_width);
            free(*objects_height);
            return;
        }
        (*objects_width)[i] = width;  
        (*objects_height)[i] = height; 
    }

    fclose(f);
}
