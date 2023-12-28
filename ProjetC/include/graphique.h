#ifndef GRAPH
#define GRAPH

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#include "moteur.h"



void afficher_plateau(Terrain *jeu);

void affiche_file_gemme(Game jeu);

void affiche_infos();

void affiche_mana(Mana mana);

int clic(Game *game);

#endif
