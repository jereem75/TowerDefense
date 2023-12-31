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
    jeu->tours.nb_tours = 0;
}



void moteur(Game *jeu){
    int selected_lvl = 0;
    int statut_t = -1;
    int statut_gem = -1;
    int val = 0;
    while (1){
        affiche_file_gemme(*jeu, statut_gem);
        affiche_infos();
        affiche_mana(jeu->mana, selected_lvl);
        affiche_tours(*jeu, statut_t);
        MLV_actualise_window();
        val = clic(jeu, &statut_t, &statut_gem, &selected_lvl);
        if (val == -1){
            break;
        }
    }
    free(jeu->plateau.chemin);
}