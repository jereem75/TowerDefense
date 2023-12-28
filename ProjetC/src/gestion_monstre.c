#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/gestion_monstre.h"
#include "../include/generation_terrain.h"


Type random_vague(){
    int nb = rand() % 10;
    if(nb <= 4){
        return NORMAL;
    }
    if(nb <= 6){
        return FOULE;
    }
    if(nb <= 8){
        return AGILE;
    }
    return BOSS;
}

void initialisation_vague(Terrain plateau, Vague *new){
    new->nb_vague ++; // nouvelle vague donc on incremente
    Type type_vague = random_vague();
    while(1){
        if( type_vague == BOSS && new->nb_vague < 5){
            type_vague = random_vague();
        }
        else{
            break;
        }
    }
    switch(type_vague){
        case NORMAL:
            new->nb_monstres = 12;
            new->vitesse = 1;
            break;
        case FOULE:
            new->nb_monstres = 24;
            new->vitesse = 1;
        case AGILE:
            new->nb_monstres = 12;
            new->vitesse = 2;
        case BOSS:
            new->nb_monstres = 2;
            new->vitesse = 1;
        default:
            fprintf(stderr, "erreur de type\n");
            return;
            
    }
    intialisation_monstre(plateau, new);
}


void intialisation_monstre(Terrain plateau, Vague *new){
    for(int i = 0; i < new->nb_monstres; i++){
        if(new->type_vague == BOSS){
           new->monstres[i].hp_initial = (int)(H * pow(1.2, new->nb_vague)); 
        }
        else{
            new->monstres[i].hp_initial = (int)(12 * H * pow(1.2, new->nb_vague));
        }
        
        new->monstres[i].hp_restants = new->monstres[i].hp_initial;
        new->monstres[i].vitesse = new->vitesse;
        new->monstres[i].teinte = rand() % 360;
        new->monstres[i].position.x = plateau.nid.x;
        new->monstres[i].position.y = plateau.nid.y;
    }

}

void liberation_monstres(Terrain plateau, Vague *vague) {
    for (int i = 0; i < vague->nb_monstres; i++) {
        vague->monstres[i].hp_initial = 0;
        vague->monstres[i].hp_restants = 0;
        vague->monstres[i].teinte = 0;
        vague->monstres[i].vitesse = 0;
        vague->monstres[i].position.x = plateau.nid.x;
        vague->monstres[i].position.y = plateau.nid.y;
    }
    vague->nb_monstres = 0;
}
