#include <stdio.h>
#include <stdlib.h>
#include "../include/gestion_mana.h"
#include "../include/gestion_monstre.h"
#include <math.h>


void intialisation_mana( Mana *mana){
    mana->nb_mana = 1500;
    mana->nb_max_mana = 2000;
    mana->niveau = 0;
}

void up_lvl_mana( Mana *mana){
    int n = (int)(500 * pow(1.4, mana->niveau));
    if(n > mana->nb_mana){
        fprintf(stderr, " nb de mana insuffisant\n");
        return;
    }
    mana->niveau ++;
    mana->nb_mana  -= n;
    mana->nb_max_mana = (int)(2000 * pow(1.4, mana->niveau));
    
}


void gain_mana(Mana *mana, Monstre monster){
    mana->nb_mana += (int)(monster.hp_initial * 0.1 * pow(1.3, mana->niveau));
}

void perte_mana(Mana *mana, Monstre monster){
    double perte = 0.15 * monster.hp_initial * pow(1.3, mana->niveau);
    if (perte > mana->nb_mana){
        mana->nb_mana = 0;
        // fin de la partie 
        return;
    }
    mana->nb_mana -= (int) perte;
}