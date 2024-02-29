#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <math.h>

#include "../include/gestion_tirs.h"
#include "../include/gestion_monstre.h"
#include "../include/generation_terrain.h"
#include "../include/gestion_gemme.h"




void initialisation_tir(Tir *tir, int x_tour, int y_tour){
    // on initialise les tirs
    tir->indice_monstre = -1;
    tir->indice_vague = -1;
    tir->nb_tirs = 0;
    tir->x =(double) x_tour;
    tir->y =(double) y_tour;
}

void update_tir(Mana *mana, Tir *tir, Lst_Vague *vague, Gemme gemme, int tour_x, int tour_y, int longueur_chemin) {
    // si on a effectue tout les tirs
    if (tir->nb_tirs == 2) {
        return;
    }
    if(tir->indice_monstre == -1 && tir->indice_vague == -1){
        trouve_cible(vague, tir);
    }
    if(tir->indice_monstre != -1 && tir->indice_vague != -1){ // si le tir a une cible
        // si le monstre est mort on renitialise ses caracteristiques
        if(vague->tab[tir->indice_vague].monstres[tir->indice_monstre].hp_restants <= 0 || vague->tab[tir->indice_vague].monstres[tir->indice_monstre].indice_case_actuelle == longueur_chemin - 1){
            tir->x = tour_x;
            tir->y = tour_y;
            tir->indice_monstre = -1;
            tir->indice_vague = -1;
        }
        // si le tire est trop proche du monstre on frappe le monstre
        if(calcul_distance_monstre_tir((*tir), vague->tab[tir->indice_vague].monstres[tir->indice_monstre].x, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].y) <= 3.0/60.0){
            tir->nb_tirs ++;
            frappe_monstre(tir, gemme, vague, &(vague)->tab[tir->indice_vague].monstres[tir->indice_monstre], mana);
            tir->x = tour_x;
            tir->y = tour_y;
            tir->indice_monstre = -1;
            tir->indice_vague = -1;
        }
        else{ // sinon on recalcule la position du tir
            tir->x = tir->x + calcul_vecteur_unitaire(*tir, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].x, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].x, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].y, tir->x) * 3.0/60.0;
            tir->y = tir->y + calcul_vecteur_unitaire(*tir, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].y, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].x, vague->tab[tir->indice_vague].monstres[tir->indice_monstre].y, tir->y) * 3.0/60.0;

        }

    }


}



int est_dans_rayon(Tir tir, int x_monstre, int y_monstre) {
    // on calcule la distance
    double distance = sqrt(pow(tir.x - x_monstre, 2.0) + pow(tir.y - y_monstre, 2.0));
    if (distance <= 3.0) {
        return 1;
    }
    return 0;
}


int degats(Gemme gemme, Tir tir, int teinte_monstre) {
    // on calcule le degat du tir
    int nb = (int)(D * pow(2.0, gemme.niveau) * (1.0 - cos(gemme.teinte - teinte_monstre) / 2.0));
    return nb;
}

double calcul_distance_monstre_tir(Tir tir, double x_monstre, double y_monstre) {
    // on calcule la distance monstre tir
    return sqrt(pow(x_monstre - tir.x, 2) + pow(y_monstre - tir.y, 2));
}


double calcul_vecteur_unitaire(Tir tir, double monstre_coord, double monstre_x, double monstre_y, double tir_coord){
    // on calcule le vecteur unitaire
    return (monstre_coord - tir_coord) / calcul_distance_monstre_tir(tir, monstre_x, monstre_y);
}

void frappe_monstre(Tir *tir, Gemme gemme, Lst_Vague *vague, Monstre *monstre, Mana *mana){
    int attaque = degats(gemme, *tir, monstre->teinte);
    int nb;
    if( monstre->effet == VIDE){ // si le monstre n'avait pas d'effet
        monstre->effet = gemme.type;
        switch(monstre->effet){ // on lui applique l'effet du tir
            case PYRO:
                applique_pyro_monstres(gemme, *tir, vague, *monstre);
                break;
            case DENDRO:
                monstre->effet = DENDRO;
                monstre->dernier_degat_dendro = attaque;
                clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet)); // on lacnce les chronos de l'effet
                clock_gettime(CLOCK_REALTIME, &(monstre->dendro));
                break;
            case HYDRO:
                monstre->effet = HYDRO;
                monstre->vitesse /= 1.5;
                clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
                break;
            case MIXTE:
                nb = rand() % 10; // on calcule une probabilite d'avoir un tir de degats doubles
                if(nb == 0){
                    attaque *= 2;
                }
                monstre->effet = VIDE;
                break;
            default:
                break;
        }
    }
    else if(monstre->effet == PYRO){ // si le monstre avait deja un effet pyro
        if(gemme.type == PYRO){
            applique_pyro_monstres(gemme, *tir, vague, *monstre); // on applique l'effet aux monstres a proximite
        }
        // sinon on applique les autres effets
        if( gemme.type == DENDRO){
            monstre->effet = PYRO_DENDRO;
            attaque *= 3;
        }
        if( gemme.type == HYDRO){
            monstre->effet = PYRO_HYDRO;
            monstre->vitesse /= 1.25;
            clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
            applique_pyro_hydro(gemme, *tir, vague, *monstre);
        }
    }
    else if( monstre->effet == HYDRO){
        if (gemme.type == HYDRO){
            clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
        }
        if(gemme.type == PYRO){
            monstre->effet = PYRO_HYDRO;
            monstre->vitesse /= 1.25;
            clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
            applique_pyro_hydro(gemme, *tir, vague, *monstre);
        }
        if(gemme.type == DENDRO){
            monstre->effet = DENDRO_HYDRO;
            monstre->vitesse = 0.0;
            clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
        }
    }
    else if(monstre->effet == DENDRO){
        // de meme pour les autres effets
        if (gemme.type == PYRO){
            monstre->effet = PYRO_DENDRO;
            attaque *= 3;
        }
        if( gemme.type == DENDRO){
            monstre->hp_restants -= attaque;
            clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
            clock_gettime(CLOCK_REALTIME, &(monstre->dendro));
        }
        if( gemme.type == HYDRO){
            monstre->vitesse = 0.0;
            clock_gettime(CLOCK_REALTIME, &(monstre->debut_effet));
            monstre->effet = DENDRO_HYDRO;
        }
    }
    monstre->hp_restants -= attaque ;
    vague->total_degats += attaque;
    if( monstre->hp_restants < 0){
        gain_mana(mana, monstre->hp_initial);
    }
}

void applique_pyro_monstres(Gemme gemme, Tir tir, Lst_Vague *vague, Monstre monstre){
    for (int i = 0; i < vague->vague_actuelle; i++) {
        for (int j = 0; j < vague->tab[i].nb_monstres; j++) {
            // on parcourt toutes les vagues et tout les monstres
            double distance = sqrt(pow(monstre.x - vague->tab[i].monstres[j].x, 2.0) + pow(monstre.y - vague->tab[i].monstres[j].y, 2.0));
            if( distance <= 2.0 && &monstre != &(vague)->tab[i].monstres[j]){ // si la distance est assez proche on applique les effets
                if(vague->tab[i].monstres[j].indice_case_actuelle != 0){
                    int perte = degats(gemme, tir, vague->tab[i].monstres[j].teinte);
                    switch(vague->tab[i].monstres[j].effet){
                        case VIDE:
                        case PYRO:
                            vague->tab[i].monstres[j].hp_restants -= 0.15 * perte;
                            vague->tab[i].monstres[j].effet = PYRO;
                            break;
                        case DENDRO:
                            vague->tab[i].monstres[j].effet = PYRO_DENDRO;
                            vague->tab[i].monstres[j].hp_restants -= 3 * perte;
                            break;
                        case HYDRO:
                            vague->tab[i].monstres[j].effet = PYRO_HYDRO;
                            vague->tab[i].monstres[j].hp_restants -= perte;
                            applique_pyro_hydro(gemme, tir, vague, vague->tab[i].monstres[j]);
                            clock_gettime(CLOCK_REALTIME, &(vague->tab[i].monstres[j].debut_effet));
                            break;
                        default:
                        vague->tab[i].monstres[j].hp_restants -= perte;;
                    }
                }
            }
        }
    }
}


void applique_pyro_hydro(Gemme gemme, Tir tir, Lst_Vague *vague, Monstre monstre){
    for (int i = 0; i < vague->vague_actuelle; i++) {
        for (int j = 0; j < vague->tab[i].nb_monstres; j++) {
            // on parcourt toutes les vagues et tout les monstres
            if( sqrt(pow(monstre.x - vague->tab[i].monstres[j].x, 2.0) + pow(monstre.y - vague->tab[i].monstres[j].y, 2.0)) <= 3.5){
                // si le monstre est assez proche on applique l'effet
                if(vague->tab[i].monstres[j].indice_case_actuelle != 0){
                    int perte = degats(gemme, tir, vague->tab[i].monstres[j].teinte);
                    vague->tab[i].monstres[j].hp_restants -= 0.05 * perte;
                    vague->tab[i].monstres[j].effet = PYRO_HYDRO;
                    vague->tab[i].monstres[j].vitesse /= 1.25;
                    clock_gettime(CLOCK_REALTIME, &(vague->tab[i].monstres[j].debut_effet));
                }

            }
        }
    }
}


void trouve_cible(Lst_Vague *vague, Tir *tir){
    int hp_max = 0;
    for (int i = 0; i < vague->vague_actuelle; i++) {
        for (int j = 0; j < vague->tab[i].nb_monstres; j++) {
            // on parcourt toutes les vagues et tout les monstres
            if (vague->tab[i].monstres[j].hp_restants > hp_max && vague->tab[i].monstres[j].hp_restants > 0 && vague->tab[i].monstres[j].indice_case_actuelle != 0) {
                // si un monstre est dans un rayon de 3 cases et n'est pas mort on affecte les coordonnees du monstre
                if (est_dans_rayon(*tir, vague->tab[i].monstres[j].x, vague->tab[i].monstres[j].y) == 1) {
                    tir->indice_monstre = j;
                    tir->indice_vague = i;
                    hp_max = vague->tab[i].monstres[j].hp_restants;
                }

            }
        }
    }
}
