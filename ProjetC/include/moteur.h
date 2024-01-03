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
    Lst_Vague vagues;
}Game;



/* fonction qui initialise la structure Game */
void initialise(Game *jeu);

/* fonction qui realise le jeu */
void moteur(Game *jeu);

#endif