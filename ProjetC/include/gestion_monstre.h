#ifndef GESTION_MO
#define GESTION_MO

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#define NB_MAX 24
#define H 2

typedef enum{
    NORMAL,
    FOULE,
    AGILE,
    BOSS
}Type;

typedef struct{
    int vitesse;
    int hp_restants;
    int hp_initial;
    int teinte;
    double x;
    double y;
    int indice_case_actuelle;
} Monstre;

typedef struct{
    Type type_vague;
    Monstre monstres[24];
    int nb_monstres;
    int vitesse;
} Vague;

typedef struct{
    Vague tab[100];
    int vague_actuelle;
}Lst_Vague;

/* fonction qui renvoie un type de vague en fonction des probabilites*/
Type random_vague(int vague_actuelle);

/* fonction qui initialise la structure Monstre*/
void intialisation_monstre(Terrain plateau, Vague *new, int nb_vague);

/* fonction qui initialise la structure Vague */
void initialisation_vague(Terrain plateau, Lst_Vague* lst);

void update_monster(Terrain plateau, Lst_Vague *lst);


#endif
