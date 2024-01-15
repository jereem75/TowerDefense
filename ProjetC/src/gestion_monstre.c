#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/resource.h>
#include "../include/gestion_monstre.h"
#include "../include/generation_terrain.h"
#include "../include/graphique.h"
#include "../include/gestion_mana.h"




Type random_vague(int vague_actuelle){
    // on calcule aleatoirement le type de vague
    int nb;
    if(vague_actuelle <= 5){ // on chage les probabilites si la vague est superieure a 5 pour integrer les boss
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
    // on calcule le type de vague
    switch(type_vague){ // on y affecte les parametres en fonction du type
        case NORMAL:
            lst->tab[lst->vague_actuelle - 1].nb_monstres = 12;
            lst->tab[lst->vague_actuelle - 1].vitesse = 1.0;
            break;
        case FOULE:
            lst->tab[lst->vague_actuelle - 1 ].nb_monstres = 24;
            lst->tab[lst->vague_actuelle - 1].vitesse = 1.0;
            break;
        case AGILE:
            lst->tab[lst->vague_actuelle - 1].nb_monstres = 12;
            lst->tab[lst->vague_actuelle - 1].vitesse = 2.0;
            break;
        case BOSS:
            lst->tab[lst->vague_actuelle - 1].nb_monstres = 2;
            lst->tab[lst->vague_actuelle - 1].vitesse = 1.0;

    }
    lst->tab[lst->vague_actuelle - 1].type_vague = type_vague;
    intialisation_monstre(plateau, &lst->tab[lst->vague_actuelle - 1], lst->vague_actuelle - 1);
    // on initialise les monstres
}


void intialisation_monstre(Terrain plateau, Vague *new, int nb_vague){
    for(int i = 0; i < new->nb_monstres; i++){
        if(new->type_vague != BOSS){ // on calcule les points de vie en fonction du type de monstre
           new->monstres[i].hp_initial = (int)(H * pow(1.2, nb_vague)); 
        }
        else{
            new->monstres[i].hp_initial = (int)(12 * H * pow(1.2, nb_vague));
        }
        // on initialise les autres caraacteristiques
        new->monstres[i].hp_restants = new->monstres[i].hp_initial;
        new->monstres[i].vitesse = new->vitesse;
        new->monstres[i].teinte = rand() % 360;
        new->monstres[i].x = plateau.nid.x;
        new->monstres[i].y = plateau.nid.y;
        new->monstres[i].indice_case_actuelle = 0;
        new->monstres[i].effet = VIDE;
    }

}

double distance_aleatoire(double vitesse) {
    int random = rand() % 101 + 10; // on prend un nombre qu'on divise ensuite pas 100
    double random_double = random / 100.0;
    double resultat = random_double * vitesse / 60.0;
    return resultat;
}

void update_monster(Terrain plateau, Lst_Vague *lst, Mana *mana){
    for(int i = 0; i < lst->vague_actuelle; i++){
        // on parcourt outes les svagues
        int n = lst->tab[i].nb_monstres;
        int statut = 0; //
        for(int j = 0; j < n; j++){
            // on parcourt tout les monstres
            float vitesse = lst->tab[i].monstres[j].vitesse;
            if(lst->tab[i].monstres[j].hp_restants <= 0){ // si un monstre n'a plus de vie
                continue;
            }
            if(lst->tab[i].type_vague == FOULE ){ // on reactualise la vitesse du type foule si ils ont depasse la premiere case
                if(lst->tab[i].monstres[j].indice_case_actuelle <= 1){
                    lst->tab[i].monstres[j].vitesse = 2;
                }
                else{
                    lst->tab[i].monstres[j].vitesse = 1;
                }
            }
            if(lst->tab[i].monstres[j].indice_case_actuelle == plateau.longueur - 1){
                // si le monstres est arrive au camp
                if(lst->tab[i].monstres[j].hp_restants > 0){
                    lst->tab[i].monstres[j].indice_case_actuelle = 0;
                    lst->tab[i].monstres[j].x = plateau.nid.x;
                    lst->tab[i].monstres[j].y = plateau.nid.y;
                    perte_mana(mana, lst->tab[i].monstres[j].hp_restants);
                    // on le replace au nid et on enleve de la mana
                }
                continue;
            }
            if(statut == 1 && lst->tab[i].monstres[j].indice_case_actuelle == 0){
                // si la premiere case est deja occupee par un monstre de la vague
                continue;
            }
            applique_effet(plateau, lst->tab[i], &(lst)->tab[i].monstres[j]);
            double distance = distance_aleatoire(vitesse);
            // on fait avancer le monstre en fonction de sa direction avec une distance aleatoire
            avance_monstre(plateau, &(lst)->tab[i].monstres[j], distance);
            if(lst->tab[i].monstres[j].indice_case_actuelle == 0){ 
                // si le monstre est a la premiere case on attend avant de faire sortir le prochain
                statut = 1;
            }
        }
    }
}


void avance_monstre(Terrain plateau, Monstre *monstre, double distance){
    switch(plateau.chemin[monstre->indice_case_actuelle].suiv){
        case HAUT:
            monstre->x -= distance;
            if(monstre->x <= plateau.chemin[monstre->indice_case_actuelle + 1].x){
                monstre->indice_case_actuelle++;
                monstre->x = plateau.chemin[monstre->indice_case_actuelle].x;
            }
            break;
        case BAS:
            monstre->x += distance;
            if(monstre->x >= plateau.chemin[monstre->indice_case_actuelle + 1].x){
                monstre->indice_case_actuelle++;
                monstre->x = plateau.chemin[monstre->indice_case_actuelle].x;
            }
            break;
        case GAUCHE:
            monstre->y -= distance;
            if(monstre->y <= plateau.chemin[monstre->indice_case_actuelle + 1].y){
                monstre->indice_case_actuelle++;
                monstre->y = plateau.chemin[monstre->indice_case_actuelle].y;
            }
            break;
        case DROITE:
            monstre->y += distance;
            if(monstre->y >= plateau.chemin[monstre->indice_case_actuelle + 1].y){
                monstre->indice_case_actuelle++;
                monstre->y = plateau.chemin[monstre->indice_case_actuelle].y;
            }
            break;
    }
}


void applique_effet(Terrain plateau, Vague vague, Monstre *monstre){
    if(monstre->effet == DENDRO){ // on actualise les monstres atteints de l'effet DENDRO
        clock_gettime(CLOCK_REALTIME, &(monstre->fin_effet));
        // si le temps superieur a 0.5 sec on applique l'effet
        if(monstre->fin_effet.tv_sec - monstre->dendro.tv_sec >= 0.5){
            clock_gettime(CLOCK_REALTIME, &(monstre->dendro));
            monstre->hp_restants -= 0.025 * monstre->dernier_degat_dendro;
        }
        // si l'effet est termine on le retire
        if(monstre->fin_effet.tv_sec - monstre->debut_effet.tv_sec >= 10){
            monstre->effet = VIDE;
        }  
    }
    // on reactualise l'effet DENDRO_HYDRO
    if(monstre->effet == DENDRO_HYDRO ){
        clock_gettime(CLOCK_REALTIME, &(monstre->fin_effet));
        if(monstre->fin_effet.tv_sec - monstre->debut_effet.tv_sec >= 3){
            monstre->effet = VIDE;
            if(vague.type_vague == AGILE){
                monstre->vitesse = 2.0;
            }
            else{
                monstre->vitesse = 1.0;
            }
        }
    }
    // on reactualise l'effet PYRO qui n'a pas de duree
    if(monstre->effet == PYRO || monstre->effet == PYRO_DENDRO){
        monstre->effet = VIDE;
    }
    // on reactualise l'effet PYRO HYDRO
    if(monstre->effet == PYRO_HYDRO ){
        clock_gettime(CLOCK_REALTIME, &(monstre->fin_effet));
        // si l'effet est termine on enlve et on remet les vitesses
        if(monstre->fin_effet.tv_sec - monstre->debut_effet.tv_sec >= 5){
            monstre->effet = VIDE;
            if(vague.type_vague == AGILE){
                monstre->vitesse = 2.0;
            }
            else{
                monstre->vitesse = 1.0;
            }
        }
    }
    // on reactualise l'effet HYDRO
    if(monstre->effet == HYDRO ){
        clock_gettime(CLOCK_REALTIME, &(monstre->fin_effet));
        // si l'effet est termine on remet les vitesses normales
        if(monstre->fin_effet.tv_sec - monstre->debut_effet.tv_sec >= 10){
            monstre->effet = VIDE;
            if(vague.type_vague == AGILE){
                monstre->vitesse = 2.0;
            }
            else{
                monstre->vitesse = 1.0;
            }
        }
    }
}