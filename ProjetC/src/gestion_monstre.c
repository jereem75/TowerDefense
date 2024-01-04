#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/gestion_monstre.h"
#include "../include/generation_terrain.h"
#include "../include/graphique.h"


Type random_vague(int vague_actuelle){
    int nb;
    if(vague_actuelle <= 5){
        nb = rand() % 8;
    }
    else{
        nb = rand() % 10;
    }
    if(nb < 4){
        return NORMAL;
    }
    if(nb < 6){
        return FOULE;
    }
    if(nb < 8){
        return AGILE;
    }
    return BOSS;
}

void initialisation_vague(Terrain plateau, Lst_Vague* lst){
    Type type_vague = random_vague(lst->vague_actuelle);
    //printf("type%d\n", type_vague);
    switch(type_vague){
        case NORMAL:
            lst->tab[lst->vague_actuelle - 1].nb_monstres = 12;
            lst->tab[lst->vague_actuelle - 1].vitesse = 1;
            break;
        case FOULE:
            lst->tab[lst->vague_actuelle - 1 ].nb_monstres = 24;
            lst->tab[lst->vague_actuelle - 1].vitesse = 1;
            break;
        case AGILE:
            lst->tab[lst->vague_actuelle - 1].nb_monstres = 12;
            lst->tab[lst->vague_actuelle - 1].vitesse = 2;
            break;
        case BOSS:
            lst->tab[lst->vague_actuelle - 1].nb_monstres = 2;
            lst->tab[lst->vague_actuelle - 1].vitesse = 1;

    }
    lst->tab[lst->vague_actuelle - 1].type_vague = type_vague;
    intialisation_monstre(plateau, &lst->tab[lst->vague_actuelle - 1], lst->vague_actuelle - 1);
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
        new->monstres[i].x = plateau.nid.x;
        new->monstres[i].y = plateau.nid.y;
        new->monstres[i].indice_case_actuelle = 0;
    }

}

double generate_random_value(double v) {
    // Utilisez srand une fois au début pour initialiser le générateur de nombres aléatoires
    // srand(time(NULL));

    // Générez un nombre entier aléatoire entre 10 et 110
    int random = rand() % 101 + 10;

    // Convertissez le facteur aléatoire en un facteur décimal
    double random_double = random / 100.0;

    // Calculez la valeur finale dans la plage spécifiée
    double result = random_double * v / 60.0;
    return result;
}

void update_monster(Terrain plateau, Lst_Vague *lst){
    for(int i = 0; i < lst->vague_actuelle; i++){
        
        int n = lst->tab[i].nb_monstres;
        //printf("nb vagues%d, nb monstres %d\n", lst->vague_actuelle, n);
        int vitesse = lst->tab[i].vitesse;
        int statut = 0;
        for(int j = 0; j < n; j++){
            if(lst->tab[i].monstres[j].indice_case_actuelle == plateau.longueur - 1){
                if(lst->tab[i].monstres[j].hp_restants > 0){
                    lst->tab[i].monstres[j].indice_case_actuelle = 0;
                    lst->tab[i].monstres[j].x = plateau.nid.x;
                    lst->tab[i].monstres[j].y = plateau.nid.y;
                }
                continue;
            }
            if(statut == 1 && lst->tab[i].monstres[j].indice_case_actuelle == 0){
                continue;
            }
            double d = generate_random_value(vitesse);
            switch(plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].suiv){
                case HAUT:
                    lst->tab[i].monstres[j].x -= d;
                    if(lst->tab[i].monstres[j].x <= plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle + 1].x){
                        lst->tab[i].monstres[j].indice_case_actuelle++;
                        lst->tab[i].monstres[j].x = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].x;
                    }
                    break;
                case BAS:
                    lst->tab[i].monstres[j].x += d;
                    if(lst->tab[i].monstres[j].x >= plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle + 1].x){
                        lst->tab[i].monstres[j].indice_case_actuelle++;
                        lst->tab[i].monstres[j].x = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].x;
                    }
                    break;
                case GAUCHE:
                    lst->tab[i].monstres[j].y -= d;
                    if(lst->tab[i].monstres[j].y <= plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle + 1].y){
                        lst->tab[i].monstres[j].indice_case_actuelle++;
                        lst->tab[i].monstres[j].y = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].y;
                    }
                    break;
                case DROITE:
                    lst->tab[i].monstres[j].y += d;
                    if(lst->tab[i].monstres[j].y >= plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle + 1].y){
                        lst->tab[i].monstres[j].indice_case_actuelle++;
                        lst->tab[i].monstres[j].y = plateau.chemin[lst->tab[i].monstres[j].indice_case_actuelle].y;
                    }
                    break;
            }
            if(lst->tab[i].monstres[j].indice_case_actuelle == 0){
                statut = 1;
            }
        }
    }
}



