#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/gestion_monstre.h"
#include "../include/generation_terrain.h"
#include "../include/graphique.h"


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

void initialisation_vague(Terrain plateau, Lst_Vague* lst){
    Type type_vague = random_vague();
    while(1){
        if( type_vague == BOSS && lst->vague_actuelle < 5){
            type_vague = random_vague();
        }
        else{
            break;
        }
    }
    printf("type%d\n", type_vague);
    switch(type_vague){
        case NORMAL:
            lst->tab[lst->vague_actuelle].nb_monstres = 12;
            lst->tab[lst->vague_actuelle].vitesse = 1;
        case FOULE:
            lst->tab[lst->vague_actuelle].nb_monstres = 24;
            lst->tab[lst->vague_actuelle].vitesse = 1;
            break;
        case AGILE:
            lst->tab[lst->vague_actuelle].nb_monstres = 12;
            lst->tab[lst->vague_actuelle].vitesse = 2;
        case BOSS:
            lst->tab[lst->vague_actuelle].nb_monstres = 2;
            lst->tab[lst->vague_actuelle].vitesse = 1;

    }
    lst->tab[lst->vague_actuelle].type_vague = type_vague;
    intialisation_monstre(plateau, &lst->tab[lst->vague_actuelle], lst->vague_actuelle);
}


void intialisation_monstre(Terrain plateau, Vague *new, int nb_vague){
    for(int i = 0; i < new->nb_monstres; i++){
        if(new->type_vague == BOSS){
           new->monstres[i].hp_initial = (int)(H * pow(1.2, nb_vague)); 
        }
        else{
            new->monstres[i].hp_initial = (int)(12 * H * pow(1.2, nb_vague));
        }
        
        new->monstres[i].hp_restants = new->monstres[i].hp_initial;
        new->monstres[i].vitesse = new->vitesse;
        new->monstres[i].teinte = rand() % 360;
        new->monstres[i].x = plateau.nid.x * LONGUEUR_CASE + CENTRE_CASE;
        new->monstres[i].y = plateau.nid.y * LONGUEUR_CASE + CENTRE_CASE;
        new->monstres[i].indice_case_actuelle = 0;
    }

}

double generate_random_value(double v) {
    // Utilisez srand une fois au début pour initialiser le générateur de nombres aléatoires
    // srand(time(NULL));

    // Générez un nombre entier aléatoire entre 10 et 110
    int random_factor = rand() % 101 + 10;

    // Convertissez le facteur aléatoire en un facteur décimal
    double random_factor_decimal = random_factor / 100.0;

    // Calculez la valeur finale dans la plage spécifiée
    double result = random_factor_decimal * v / 60.0;

    return result;
}

void update_monster(Terrain plateau, Lst_Vague *lst){
    for(int i = 0; i < lst->vague_actuelle; i++){
        int n = lst->tab[i].nb_monstres;
        int vitesse = lst->tab[i].vitesse;
        for(int j = 0; j < n; j++){
            double d = generate_random_value(vitesse);
            switch(plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].suiv){
            case HAUT:
                lst->tab->monstres[i].x -= d;
                if(plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].x * LONGUEUR_CASE + CENTRE_CASE > lst->tab[i].monstres[j].x *LONGUEUR_CASE + CENTRE_CASE){
                    lst->tab[i].monstres[j].x = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].x * LONGUEUR_CASE / 2;
                    lst->tab[i].monstres[j].indice_case_actuelle ++;
                }
            case BAS:
                lst->tab->monstres[i].x += d;
                if(plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].x * LONGUEUR_CASE + CENTRE_CASE < lst->tab[i].monstres[j].x *LONGUEUR_CASE + CENTRE_CASE){
                    lst->tab[i].monstres[j].x = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].x * LONGUEUR_CASE / 2;
                    lst->tab[i].monstres[j].indice_case_actuelle ++;
                }
            case GAUCHE:
                lst->tab->monstres[i].y -= d;
                if(plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].y * LONGUEUR_CASE + CENTRE_CASE > lst->tab[i].monstres[j].y *LONGUEUR_CASE + CENTRE_CASE){
                    lst->tab[i].monstres[j].y = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].y * LONGUEUR_CASE / 2;
                    lst->tab[i].monstres[j].indice_case_actuelle ++;
                }
            case DROITE:
                lst->tab->monstres[i].x += d;
                if(plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].y * LONGUEUR_CASE + CENTRE_CASE < lst->tab[i].monstres[j].y *LONGUEUR_CASE + CENTRE_CASE){
                    lst->tab[i].monstres[j].y = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].y * LONGUEUR_CASE / 2;
                    lst->tab[i].monstres[j].indice_case_actuelle ++;
                }
            }
        }
    }
}

