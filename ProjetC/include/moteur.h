#ifndef MOTEUR
#define MOTEUR

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#include "gestion_gemme.h"
#include "gestion_tour.h"
#include "moteur.h"


typedef struct{
    Terrain plateau;
    Liste_gemme gemmes;
    Mana mana;
    Lst_Tour tours;
}Game;

void initialise(Game *jeu);

void moteur(Game *jeu);

#endif