#include <stdio.h>
#include <stdlib.h>
#include "../include/moteur.h"
#include "../include/gestion_gemme.h"
#include "../include/generation_terrain.h"
#include "../include/graphique.h"
#include <MLV/MLV_all.h>


void initialise(Game *jeu){
    intialisation_mana(&(jeu)->mana);
    intialise_gemme(&(jeu)->gemmes);
    initialise_cases(&(jeu)->plateau);
}



void moteur(Game *jeu){
    int val = 0;
    while (1){
        affiche_file_gemme(*jeu);
        affiche_infos();
        affiche_mana(jeu->mana);
        MLV_actualise_window();
        val = clic(jeu);
        if (val == -1){
            break;
        }
    }
    
}