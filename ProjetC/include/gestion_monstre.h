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
    Case position;
} Monstre;

typedef struct{
    Type type_vague;
    int nb_vague;
    Monstre monstres[24];
    int nb_monstres;
    int vitesse;
} Vague;

/* fonction qui renvoie un type de vague en fonction des probabilites*/
Type random_vague();

/* fonction qui initialise la structure Monstre*/
void intialisation_monstre(Terrain plateau, Vague *new);

/* fonction qui initialise la structure Vague */
void initialisation_vague(Terrain plateau, Vague *new);

/* fonction qui libere la liste de monstre */
void liberation_monstres(Terrain plateau, Vague *vague);

#endif
