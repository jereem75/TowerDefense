#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/gestion_mana.h"



void initialisation_mana( Mana *mana){ // on initalise la mana avec les valeurs de base
    mana->nb_mana = 150;
    mana->nb_max_mana = 2000;
    mana->niveau = 0;
}

void up_lvl_mana( Mana *mana){ // on calcule prix pour aumenter la mana
    int n = (int)(500 * pow(1.4, mana->niveau));
    if(n > mana->nb_mana){ // si on a pas assez de mana
        fprintf(stderr, " nb de mana insuffisant\n");
        return;
    }
    mana->niveau ++;
    mana->nb_mana  -= n;
    mana->nb_max_mana = (int)(2000 * pow(1.4, mana->niveau));
    //on met a jour la mana
    
}


void gain_mana(Mana *mana, int hp_initial){
    // on calcule mana gagne
    double gain = hp_initial * 0.1 * pow(1.3, mana->niveau);
    mana->nb_mana += (int)(gain);
}

void perte_mana(Mana *mana, int hp_initial){
    // on calcule la mana perdue
    double perte = 0.15 * hp_initial * pow(1.3, mana->niveau);
    mana->nb_mana -= (int) perte;
}