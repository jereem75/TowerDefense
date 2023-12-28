#ifndef GESTION_TO
#define GESTION_TO
#define MAX_TOURS 541
#include <stdlib.h>
#include <stdio.h>
#include "gestion_gemme.h"
#include "generation_terrain.h"
#include "gestion_mana.h"

typedef struct{
    Case position;
    Gemme gem;
}Tour;

typedef struct{
    Tour lst_tours[MAX_TOURS];
    int nb_tours;
}Lst_Tour;


/* fonction qui renvoie l'indice de la tour dans la liste si la case est une tour 0 sinon  */
int est_case_tour(Lst_Tour lst, Case a);

/* fonction qui construit une tour si cela est possible */
void build_tour(Terrain plateau, Mana *gestion, Lst_Tour *lst, int x, int y);

#endif
