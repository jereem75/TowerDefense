#ifndef GESTION_TO
#define GESTION_TO

#define MAX_TOURS 541

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gestion_gemme.h"
#include "generation_terrain.h"
#include "gestion_mana.h"
#include "gestion_tirs.h"


typedef struct{
    Case position;
    Gemme gem;
    Tir tir;
    struct timespec debut_tir, fin_tir;
    struct timespec debut_gem, fin_gem;
}Tour;

typedef struct{
    Tour lst_tours[MAX_TOURS];
    int nb_tours;
}Lst_Tour;



/* fonction qui renvoie l'indice de la tour dans la liste si la case est une tour 0 sinon  */
int est_case_tour(Lst_Tour lst, Case a);

/* fonction qui construit une tour si cela est possible */
void build_tour(Terrain plateau, Mana *gestion, Lst_Tour *lst, int x, int y);

/* fonction qui ajoute la gem pris en parametre dans la tour*/
void add_gemme_tour(Liste_gemme *lst_gem,Lst_Tour *lst,Gemme *gem, int x, int y);

/* fonction qui met ajour les tours notemment le temps de chargement des nouvelles gemmes */
void udpate_tours(Mana *mana, Lst_Tour *lst, Lst_Vague *vague, int longueur_chemin);

#endif
