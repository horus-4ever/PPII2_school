#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "npc.h"

int dx[] = {0, 0, -1, 1};
int dy[] = {1, -1, 0, 0};
char* directions[] = {"D", "U", "L", "R"};

// Function to check if a given position is valid
bool isValid(int x, int y, int** background, int** objects, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols && (objects[x][y] == 0 || objects[x][y] == -2));
}

// Node structure to store the position and path
typedef struct {
    int x, y;
    char* path;
} Node;

// Function to find the shortest path
char** closestPath(int** background, int** objects, int rows, int cols, int startx, int starty, int destx, int desty, int* returnSize) {
    // Create a visited matrix
    bool** visited = (bool**)malloc(rows * sizeof(bool*));
    for (int i = 0; i < rows; i++) {
        visited[i] = (bool*)calloc(cols, sizeof(bool));
    }

    // Create an ArrayList for BFS
    ArrayList* queue = ArrayList_new(NULL);

    // Start from the initial position
    Node* startNode = malloc(sizeof(Node));
    startNode->x = startx;
    startNode->y = starty;
    startNode->path = (char*)calloc(1, sizeof(char));
    ArrayList_append(queue, startNode);
    visited[startx][starty] = true;

    while (ArrayList_length(queue) > 0) {
        Node* current = ArrayList_get(queue, 0);
        ArrayList_removeAt(queue, 0);

        // If the destination is reached
        if (current->x == destx && current->y == desty) {
            // Convert the path to a list of strings
            int pathLength = strlen(current->path);
            char** result = (char**)malloc(pathLength * sizeof(char*));
            for (int i = 0; i < pathLength; i++) {
                result[i] = (char*)malloc(2 * sizeof(char));
                result[i][0] = current->path[i];
                result[i][1] = '\0';
            }
            *returnSize = pathLength;

            // Free memory
            for (int i = 0; i < rows; i++) {
                free(visited[i]);
            }
            free(visited);
            for(size_t i = 0 ; i < ArrayList_length(queue) ; i += 1) {
                Node* node = ArrayList_get(queue, i);
                free(node->path);
                free(node);
            }
            ArrayList_delete(queue);
            free(current->path);
            free(current);
            return result;
        }

        // Explore all possible movements
        for (int i = 0; i < 4; i++) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (isValid(newX, newY, background, objects, rows, cols) && !visited[newX][newY]) {
                visited[newX][newY] = true;
                char* newPath = (char*)malloc((strlen(current->path) + 2) * sizeof(char));
                strcpy(newPath, current->path);
                strcat(newPath, directions[i]);

                Node* nextNode = malloc(sizeof(Node));
                nextNode->x = newX;
                nextNode->y = newY;
                nextNode->path = newPath;
                ArrayList_append(queue, nextNode);
            }
        }

        // Free the path string of the current node
        free(current->path);
        free(current);
    }

    // No path found
    *returnSize = 0;

    // Free memory
    for (int i = 0; i < rows; i++) {
        free(visited[i]);
    }
    for(size_t i = 0 ; i < ArrayList_length(queue) ; i += 1) {
        Node* node = ArrayList_get(queue, i);
        free(node->path);
        free(node);
    }
    free(visited);
    ArrayList_delete(queue);
    return NULL;
}

MoveState* init_moveState() {
    MoveState* moveState = malloc(sizeof(MoveState));
    
    moveState->up = 0;
    moveState->down = 0;
    moveState->left = 0;
    moveState->right = 0;

    return moveState;
}

MoveState* nextMove(MoveState* moveState) {
    // init
    if (moveState->up == 0 && moveState->down == 0 && moveState->left == 0 && moveState->right == 0) {
        char moves[] = {'U', 'D', 'L', 'R'};
        int random_i = rand() % 4;

        switch (moves[random_i]) {
            case 'U': 
                moveState->up = 1;
                moveState->nextDirection = 'U';
                break;

            case 'D': 
                moveState->down = 1;
                moveState->nextDirection = 'D';
                break;

            case 'L': 
                moveState->left = 1;
                moveState->nextDirection = 'L';
                break;

            case 'R': 
                moveState->right = 1;
                moveState->nextDirection = 'R';
                break;
        }
    }

    // up, down, left, right
    else if (moveState->up == 1 && moveState->down == 0 && moveState->left == 0 && moveState->right == 0) {
        char moves[] = {'D', 'L', 'R'};
        int random_i = rand() % 3;

        switch (moves[random_i]) {
            case 'D': 
                moveState->up = 0;
                moveState->nextDirection = 'D';
                break;

            case 'L': 
                moveState->left = 1;
                moveState->nextDirection = 'L';
                break;

            case 'R': 
                moveState->right = 1;
                moveState->nextDirection = 'R';
                break;
        }
    }

    else if (moveState->up == 0 && moveState->down == 1 && moveState->left == 0 && moveState->right == 0) {
        char moves[] = {'U', 'L', 'R'};
        int random_i = rand() % 3;

        switch (moves[random_i]) {
            case 'U': 
                moveState->down = 0;
                moveState->nextDirection = 'U';
                break;

            case 'L': 
                moveState->left = 1;
                moveState->nextDirection = 'L';
                break;

            case 'R': 
                moveState->right = 1;
                moveState->nextDirection = 'R';
                break;
        }
    }

    else if (moveState->up == 0 && moveState->down == 0 && moveState->left == 1 && moveState->right == 0) {
        char moves[] = {'U', 'D', 'R'};
        int random_i = rand() % 3;

        switch (moves[random_i]) {
            case 'U': 
                moveState->up = 1;
                moveState->nextDirection = 'U';
                break;

            case 'D': 
                moveState->down = 1;
                moveState->nextDirection = 'D';
                break;

            case 'R': 
                moveState->left = 0;
                moveState->nextDirection = 'R';
                break;
        }
    }

    else if (moveState->up == 0 && moveState->down == 0 && moveState->left == 0 && moveState->right == 1) {
        char moves[] = {'U', 'D', 'L'};
        int random_i = rand() % 3;

        switch (moves[random_i]) {
            case 'U': 
                moveState->up = 1;
                moveState->nextDirection = 'U';
                break;

            case 'D': 
                moveState->down = 1;
                moveState->nextDirection = 'D';
                break;

            case 'L': 
                moveState->right = 0;
                moveState->nextDirection = 'L';
                break;
        }
    }

    // up left, up right, down left, down right
    else if (moveState->up == 1 && moveState->down == 0 && moveState->left == 1 && moveState->right == 0) {
        char moves[] = {'D', 'R'};
        int random_i = rand() % 2;

        switch (moves[random_i]) {
            case 'D': 
                moveState->up = 0;
                moveState->nextDirection = 'D';
                break;

            case 'R': 
                moveState->left = 0;
                moveState->nextDirection = 'R';
                break;
        }
    }

    else if (moveState->up == 1 && moveState->down == 0 && moveState->left == 0 && moveState->right == 1) {
        char moves[] = {'D', 'L'};
        int random_i = rand() % 2;

        switch (moves[random_i]) {
            case 'D': 
                moveState->up = 0;
                moveState->nextDirection = 'D';
                break;

            case 'L': 
                moveState->right = 0;
                moveState->nextDirection = 'L';
                break;
        }
    }

    else if (moveState->up == 0 && moveState->down == 1 && moveState->left == 1 && moveState->right == 0) {
        char moves[] = {'U', 'R'};
        int random_i = rand() % 2;

        switch (moves[random_i]) {
            case 'U': 
                moveState->down = 0;
                moveState->nextDirection = 'U';
                break;

            case 'R': 
                moveState->left = 0;
                moveState->nextDirection = 'R';
                break;
        }
    }

    else if (moveState->up == 0 && moveState->down == 1 && moveState->left == 0 && moveState->right == 1) {
        char moves[] = {'U', 'L'};
        int random_i = rand() % 2;

        switch (moves[random_i]) {
            case 'U': 
                moveState->down = 0;
                moveState->nextDirection = 'U';
                break;

            case 'L': 
                moveState->right = 0;
                moveState->nextDirection = 'L';
                break;
        }
    }

    return moveState;
}

void* thread_function(void* arg) {
    // Convertit l'argument en un pointeur vers la structure ThreadArgs
    ThreadArgs* threadArgs = (ThreadArgs*) arg;

    // Récupère les différents éléments de la structure ThreadArgs
    MoveState* moveState = threadArgs->moveState;
    ArrayList* npcList = threadArgs->npcs;
    // Perso_SDL* player = threadArgs->player;

    // Boucle infinie pour mettre à jour le mouvement des NPCs
    while (1) {
        // Calcule le prochain mouvement en utilisant la fonction nextMove
        break;
        // Appelez votre fonction et récupérez le résultat
        MoveState* result = nextMove(moveState);

        // Récupère le premier NPC de la liste des NPCs
        Perso_SDL* perso = ((Character*) ArrayList_get(npcList, 0))->sdlPerso;

        // Met à jour la position du NPC en fonction de la direction suivante
        switch (result->nextDirection) {
            case 'U':
                update_position_specific_npc(perso, perso->x, perso->y - PERSO_SPEED);
                break;
            case 'D':
                update_position_specific_npc(perso, perso->x, perso->y + PERSO_SPEED);
                break;
            case 'L':
                update_position_specific_npc(perso, perso->x - PERSO_SPEED, perso->y);
                break;
            case 'R':
                update_position_specific_npc(perso, perso->x + PERSO_SPEED, perso->y);
                break;
        }
        // Réinitialise l'animation du personnage après avoir mis à jour la position
        reset_animation_perso(perso);

        // Fais une pause de 3 secondes avant de continuer la boucle i.e le temps d'attente entre les déplacements
        sleep(3);
    }
    
    return NULL;
}


NPCList* init_list() {
    return NULL;
}

void add_perso(NPCList** head, Perso_SDL* newPerso) {
    NPCList* newNode = (NPCList*)malloc(sizeof(NPCList));
    if (newNode == NULL) {
        fprintf(stderr, "Erreur : Échec de l'allocation de mémoire\n");
        return;
    }
    newNode->perso = newPerso;
    newNode->next = *head;
    *head = newNode;
}

void remove_perso(NPCList** head, int x, int y) {
    NPCList* current = *head;
    NPCList* prev = NULL;

    while (current != NULL) {
        if (current->perso->x == x && current->perso->y == y) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_list(NPCList* head) {
    NPCList* current = head;
    while (current != NULL) {
        NPCList* next = current->next;
        free(current->perso);
        free(current);
        current = next;
    }
}

void update_position_all_npc(ArrayList* npcs, int x, int y) {
    for(size_t i = 0 ; i < ArrayList_length(npcs) ; i += 1) {
        Perso_SDL* perso = ((Character*) ArrayList_get(npcs, i))->sdlPerso;
        perso->dstRect.x += x;
        perso->dstRect.y += y;
    }  
}

void update_position_specific_npc(Perso_SDL* npc, int x, int y) {
    int w = get_width_spritesheet(npc);
    int h = get_height_spritesheet(npc);

    if(npc->x > x) {
        npc->srcRect.y = (h/5)*2;
        // npc->dstRect.x -= PERSO_SPEED;
        npc->orientation = 4;
    } else if(npc->x < x) {
        npc->srcRect.y = (h/5)*4;
        // npc->dstRect.x += PERSO_SPEED;
        npc->orientation = 2;
    } else if(npc->y > y) {
        npc->srcRect.y = (h/5)*3;
        // npc->dstRect.y -= PERSO_SPEED;
        npc->orientation = 1;
    } else if(npc->y < y) {
        npc->srcRect.y = (h/5);
        // npc->dstRect.y += PERSO_SPEED;
        npc->orientation = 3;
    }
    npc->srcRect.x += (w/8);
    npc->srcRect.x %= w;

    npc->dstRect.x += PERSO_SPEED * (x - npc->x) / PERSO_SPEED;
    npc->dstRect.y += PERSO_SPEED * (y - npc->y) / PERSO_SPEED;
    npc->x = x;
    npc->y = y;

    
}

void NPC_SDL_RenderCopy(ArrayList* npcs, SDL_Renderer* renderer, Perso_SDL** clothes_perso, Perso_SDL* perso) {
    ArrayList* list = ArrayList_new(NULL);
    ArrayList* sorted = ArrayList_new(NULL);
    for(size_t i = 0 ; i < ArrayList_length(npcs) ; i += 1) {
        Perso_SDL* perso = ((Character*) ArrayList_get(npcs, i))->sdlPerso;
        ArrayList_append(list, perso);
    }
    ArrayList_append(list, perso);
    // sort the list
    while(ArrayList_length(list) != 0) {
        size_t minIndex = 0;
        Perso_SDL* minPointer = ArrayList_get(list, 0);
        int minimum = minPointer->y;
        for(size_t i = 0 ; i < ArrayList_length(list) ; i += 1) {
            Perso_SDL* perso = ArrayList_get(list, i);
            if(perso->y < minimum) {
                minIndex = i;
                minPointer = perso;
                minimum = perso->y;
            }
        }
        ArrayList_append(sorted, minPointer);
        ArrayList_removeAt(list, minIndex);
    }
    // draw the things
    for(size_t i = 0 ; i < ArrayList_length(sorted) ; i += 1) {
        Perso_SDL* p = ArrayList_get(sorted, i);
        SDL_RenderCopy(renderer, p->sprite, &p->srcRect, &p->dstRect);
        if(p->y == perso->y) {
            for(int i = 0 ; i < 4 ; i++){
                SDL_RenderCopy(renderer, clothes_perso[i]->sprite, &(clothes_perso[i]->srcRect), &(clothes_perso[i]->dstRect));
            }
        }
    }
    ArrayList_delete(list);
    ArrayList_delete(sorted);
    /* SDL_RenderCopy(renderer, perso->sprite, &perso->srcRect, &perso->dstRect);
    for(int i = 0 ; i < 4 ; i++){
        SDL_RenderCopy(renderer, clothes_perso[i]->sprite, &(clothes_perso[i]->srcRect), &(clothes_perso[i]->dstRect));
    } */
}

void reset_animation_npc(ArrayList* npcs){
    for(size_t i = 0 ; i < ArrayList_length(npcs) ; i += 1) {
        Perso_SDL* perso = ((Character*) ArrayList_get(npcs, 1))->sdlPerso;
        perso->srcRect.x = 0;
    }
}