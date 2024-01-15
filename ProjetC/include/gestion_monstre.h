#ifndef GESTION_MO
#define GESTION_MO

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#include "gestion_mana.h"
#include "gestion_gemme.h"
#include <time.h>
#define NB_MAX 24
#define H 10
#define MAX_VAGUES 1000

typedef enum{
    NORMAL,
    FOULE,
    AGILE,
    BOSS
}Type;


typedef struct{
    double vitesse; // vitesse  du monstre
    int hp_restants; // vie restante
    int hp_initial; // vie maximale
    int dernier_degat_dendro; // valeur du degat de tir dendro
    int teinte;
    double x; // coordonees
    double y;
    int indice_case_actuelle; // nb indiquant la case du monstre actuelle
    Type_Gemme effet;
    struct timespec debut_effet, fin_effet, dendro; // timer pour les effets
} Monstre;

typedef struct{
    Type type_vague;
    Monstre monstres[24]; // tableau de monstres
    int nb_monstres;
    double vitesse; // vitesse initiale de la vague
} Vague;

typedef struct{
    Vague tab[MAX_VAGUES]; // tableau de vagues
    int vague_actuelle; // indice de la vague actuelle
    int total_degats;
}Lst_Vague;

/* fonction qui renvoie un type de vague en fonction des probabilites*/
Type random_vague(int vague_actuelle);

/* fonction qui initialise la structure Monstre*/
void intialisation_monstre(Terrain plateau, Vague *new, int nb_vague);

/* fonction qui initialise la structure Vague */
void initialisation_vague(Terrain plateau, Lst_Vague* lst);

/* fonction qui met a ajour les monstres et modifie la mana en fonction de si le monstre est tue o
ou est arrive au nid*/
void update_monster(Terrain plateau, Lst_Vague *lst, Mana *mana);

/* fonction qui calcule une distance aleatoire en fonction de la vitesse*/
double distance_aleatoire(double vitesse);

/* fonction qui fait avancer le monstre*/
void avance_monstre(Terrain plateau, Monstre *monstre, double distance);

/* fonction qui actualise les efets appliques au monstre */
void applique_effet(Terrain plateau, Vague vague, Monstre *monstre);

#endif
