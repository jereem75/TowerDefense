#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "unistd.h"

#include "../include/generation_terrain.h"
#include "../include/graphique.h"

int distance_manhattan(Case i, Case j){
    return abs(i.x - j.x) + abs(i.y - j.y);
}

void random_nid(Terrain *plateau){
    int x;
    int y;
    while(1){
        x = rand() % LIG;
        y = rand() % COL;
        if(x < 2 || x >= LIG - 2){ // on verifie qu'il n'est pas a deux cases du bord
            continue;
        }
        if(y < 2 || y >= COL - 2){
            continue;
        }
        plateau->nid.x = x; // on initalise le nid
        plateau->nid.y = y;
        plateau->nid.suiv = HAUT;
        break;
    }
}


int est_definie(Case a){
    if(a.x < 2 || a.x >= LIG - 2 || a.y < 2 || a.y >= COL - 2){ // on regarde si la case est definie
        return 0;
    }
    return 1;
}


int etendu(Terrain plateau, Case a, Direction dir) {
    int longueur = 0;
    Case suiv;
    switch (dir) { // on calcule l'etendue pour chaque direction
        case HAUT:
            suiv.x = a.x - 1;
            suiv.y = a.y;
            while (est_definie(suiv) == 1 && compare_chemin(suiv, plateau) == 0) {
                longueur++;
                suiv.x = suiv.x - 1;
            }
            break;
        case BAS:
            suiv.x = a.x + 1;
            suiv.y = a.y;
            while (est_definie(suiv) == 1 && compare_chemin(suiv, plateau) == 0) {
                longueur++;
                suiv.x = suiv.x + 1;
            }
            break;
        case GAUCHE:
            suiv.x = a.x;
            suiv.y = a.y - 1;
            while (est_definie(suiv) == 1 && compare_chemin(suiv, plateau) == 0) {
                longueur++;
                suiv.y = suiv.y - 1;
            }
            break;
        case DROITE:
            suiv.x = a.x;
            suiv.y = a.y + 1;
            while (est_definie(suiv) == 1 && compare_chemin(suiv, plateau) == 0) {
                longueur++;
                suiv.y = suiv.y + 1;
            }
            break;
        default:
        fprintf(stderr,"erreur\n");
            return 0;
    }
    return longueur;
}





int compare_chemin(Case a, Terrain plateau){
    for(int i = 0; i < plateau.longueur - 2; i++ ){ // on regarde si les cases sont suffisament eloignée du chemin
        if(distance_manhattan(a, plateau.chemin[i]) <= 2){
            return 1;
        }
    }
    return 0;
}


Direction direction_initiale(Terrain plateau, Case nid) {
    // on calcule l'etendue pour chaque direction
    int etendu_haut = etendu(plateau, nid, HAUT);
    int etendu_bas = etendu(plateau, nid, BAS);
    int etendu_gauche = etendu(plateau, nid, GAUCHE);
    int etendu_droite = etendu(plateau, nid, DROITE);
    int somme_etendus = 0;
    somme_etendus = etendu_haut + etendu_bas + etendu_gauche + etendu_droite;
    int random_num = rand() % somme_etendus; // on choisit une direction au hasard en fonction des etendues
    if (random_num < etendu_haut) {
        return HAUT;
    } else if (random_num < etendu_haut + etendu_bas) {
        return BAS;
    } else if (random_num < etendu_haut + etendu_bas + etendu_gauche) {
        return GAUCHE;
    } else {
        return DROITE;
    }
}


void generation(Terrain *plateau){
    plateau->nb_virages = 0;
    plateau->longueur = 0;
    plateau->chemin = NULL;
    random_nid(plateau);
    plateau->chemin = (Case *)malloc(sizeof(Case) * 85); // on alloue au minimum 85 cases
    if (plateau->chemin == NULL) {
        fprintf(stderr, "erreur d'allocation\n");
        return;
    }
    plateau->chemin[0] = plateau->nid; // on affecte le nid
    plateau->longueur ++;
    Direction dir = direction_initiale(*plateau, plateau->nid); // on choisit une direction initiale
    Case initial = plateau->nid;
    while (plateau->longueur < 85 || plateau->nb_virages <= 7) { // tant que le chemin n'est pas termine
        int n = etendu(*plateau, initial, dir); // on calcule l'etendue en fonction de la direction
        if (n <= 2) {
            // si inferieur ou egal a deux on recommence
            free(plateau->chemin); // on libere la memoire du precedent chemin
            plateau->chemin = NULL;
            plateau->longueur = 0;
            plateau->nb_virages = 0;
            generation(plateau); // on recommence
            return;
        }
        int s = 0;
        int nb = 0;
        for (int i = 0; i < n; i++) { // on choisit un nombres de cases au hasard
            nb = rand() % 4;
            if (nb < 3) {
                s += 1;
            }
        }
        if(s < 3){
            s = 3;
        }
        ajoute_cases_chemin(plateau, initial, dir, s - 1); // on ajoutes les cases au chemin
        if(prochaine_direction(*plateau, dir, &dir) == -1){ // on tire la prochaine direction
            free(plateau->chemin); // on libere la memoire du precedent chemin
            plateau->chemin = NULL;
            plateau->longueur = 0;
            plateau->nb_virages = 0;
            generation(plateau); // on recommence
            return;
        }
        initial = plateau->chemin[plateau->longueur - 1];
        plateau->nb_virages ++;
    }
    plateau->nb_virages  --;
}


void ajoute_cases_chemin(Terrain *plateau, Case a, Direction dir, int nb_cases){
    Case suiv;
    for (int i = 1; i <= nb_cases + 1; i++) { // on boucle pour ajouter nb_cases
        switch (dir) {
            case HAUT:
                suiv.x = a.x - i;
                suiv.y = a.y;
                break;
            case BAS:
                suiv.x = a.x + i;
                suiv.y = a.y;
                break;
            case GAUCHE:
                suiv.x = a.x;
                suiv.y = a.y - i;
                break;
            case DROITE:
                suiv.x = a.x;
                suiv.y = a.y + i;
                break;
            default:
                fprintf(stderr, "Direction invalide\n");
                return;
        }
        if(plateau->longueur >= 85){ // si le chemin n'est pas assez grand on realloue
            plateau->chemin = (Case*)realloc(plateau->chemin, sizeof(Case) * (plateau->longueur + 1));
            if(plateau->chemin == NULL){
                fprintf(stderr, "Erreur d'allocation\n");
                return;
            }
        }
        plateau->chemin[plateau->longueur - 1].suiv = dir;
        suiv.suiv = dir;
        plateau->chemin[plateau->longueur] = suiv;
        plateau->longueur ++;
        // on ajoute les cases

    }
}


int prochaine_direction(Terrain plateau, Direction derniere_dir, Direction *next){
    int etendu_droite, etendu_gauche;
    int somme = 0;
    switch (derniere_dir) { // en fonction de la derniere direction on calcule la prochaine avec l'etendu
            case HAUT:
                etendu_droite = etendu(plateau, plateau.chemin[plateau.longueur - 1], DROITE);
                etendu_gauche = etendu(plateau, plateau.chemin[plateau.longueur - 1], GAUCHE);
                break;
            case BAS:
                etendu_droite = etendu(plateau, plateau.chemin[plateau.longueur - 1], GAUCHE);
                etendu_gauche = etendu(plateau, plateau.chemin[plateau.longueur - 1], DROITE);
                break;
            case GAUCHE:
                etendu_droite = etendu(plateau, plateau.chemin[plateau.longueur - 1], HAUT);
                etendu_gauche = etendu(plateau, plateau.chemin[plateau.longueur - 1], BAS);
                break;
            case DROITE:
                etendu_droite = etendu(plateau, plateau.chemin[plateau.longueur - 1], BAS);
                etendu_gauche = etendu(plateau, plateau.chemin[plateau.longueur - 1], HAUT);
                break;
            default:
                fprintf(stderr, "Direction invalide\n");
                return HAUT;
    }
    somme = etendu_droite + etendu_gauche;
    if(somme == 0){
      return -1;
    }
    int nb = rand() % somme;
    if (nb < etendu_droite){
        switch (derniere_dir){ // on l'a renvoie
            case HAUT:
                *next = HAUT;
                break;
            case BAS:
                *next = GAUCHE;
                break;
            case DROITE:
                *next = BAS;
                break;
            case GAUCHE:
                *next = HAUT;
                break;
        }
    }
    switch (derniere_dir){
            case HAUT:
                *next = GAUCHE;
                break;
            case BAS:
                *next = DROITE;
                break;
            case DROITE:
                *next = HAUT;
                break;
            case GAUCHE:
                *next = BAS;
                break;
        }
    return 1;
}


int est_case_chemin(Terrain plateau, Case a){
    if(plateau.chemin == NULL){
        return 0;
    }
    for(int i = 0; i < plateau.longueur; i++){ // on regarde si la case est une case du chemin
        if(plateau.chemin[i].x == a.x && plateau.chemin[i].y == a.y){
            return 1;
        }
    }
    return 0;
}

