#include <stdio.h>
#include <stdlib.h>
#include "../include/generation_terrain.h"
#include <math.h>
#include <time.h>
#include "unistd.h"
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
        if(x < 3 || x > LIG - 3){
            continue;
        }
        if(y < 3 || y > COL - 3){
            continue;
        }
        plateau->nid.x = x;
        plateau->nid.y = y;
        plateau->nid.suiv = HAUT;
        break;
    }
}

void initialise_cases(Terrain *plateau) {
    for (int i = 0; i < LIG; ++i) {
        for (int j = 0; j < COL; ++j) {
            plateau->tab[i][j].x = i;
            plateau->tab[i][j].y = j;
        }
    }
}

void afficherTableau(Terrain plateau) {
    for (int i = 0; i < LIG; ++i) {
        for (int j = 0; j < COL; ++j) {
            printf("(%d,%d) ", plateau.tab[i][j].x, plateau.tab[i][j].y);
        }
        printf("\n");
    }
}

int est_definie(Case a){
    if(a.x < 2 || a.x >= LIG - 2 || a.y < 2 || a.y >= COL - 2){
        return 0;
    }
    return 1;
}


int etendu(Terrain plateau, Case a, Direction dir) {
    int longueur = 0;
    Case suiv;
    switch (dir) {
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
        printf("erreur\n");
            return 0;
    }
    return longueur;
}





int compare_chemin(Case a, Terrain plateau){
    for(int i = 0; i < plateau.longueur - 2; i++ ){
        if(distance_manhattan(a, plateau.chemin[i]) <= 2){
            return 1;
        }
    }
    return 0;
}


Direction direction_initiale(Terrain plateau, Case nid) {
    int etendu_haut = etendu(plateau, nid, HAUT);
    int etendu_bas = etendu(plateau, nid, BAS);
    int etendu_gauche = etendu(plateau, nid, GAUCHE);
    int etendu_droite = etendu(plateau, nid, DROITE);
    int somme_etendus = 0;
    somme_etendus = etendu_haut + etendu_bas + etendu_gauche + etendu_droite;
    int random_num = rand() % somme_etendus;
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
    plateau->chemin = (Case *)malloc(sizeof(Case) * 75);
    if (plateau->chemin == NULL) {
        fprintf(stderr, "erreur d'allocation\n");
        return;
    }
    plateau->chemin[0] = plateau->nid;
    plateau->longueur ++;
    Direction dir = direction_initiale(*plateau, plateau->nid);
    Case initial = plateau->nid;
    while (plateau->longueur < 75 || plateau->nb_virages <= 7) {
        //printf("boucle\n");
        int n = etendu(*plateau, initial, dir);
        //printf("etendue %d\n", n);
        //printf("longueur = %d\n", plateau->longueur);
        if (n <= 2) {
            
            printf("erreur\n");
            free(plateau->chemin);
            plateau->chemin = NULL;
            plateau->longueur = 0;
            plateau->nb_virages = 0;
            generation(plateau);
            return;
        }
        //printf("etendue = %d\n", n);
        int s = 0;
        int nb = 0;
        for (int i = 0; i < n; i++) {
            nb = rand() % 4;
            if (nb < 3) {
                s += 1;
            }
        }
        if(s < 3){
            s = 3;
        }
       //printf("nb_cases = %d\n", s);
        ajoute_cases_chemin(plateau, initial, dir, s - 1);
        dir = prochaine_direction(*plateau, dir);
        initial = plateau->chemin[plateau->longueur - 1];
        plateau->nb_virages ++;
        //printf("%d, %d\n", plateau->longueur, plateau->nb_virages);
        //affiche_chemin(*plateau);
        //sleep(1);
    }
    plateau->nb_virages  --;
    //test(*plateau);
}


void ajoute_cases_chemin(Terrain *plateau, Case a, Direction dir, int nb_cases){
    Case suiv;
    for (int i = 1; i <= nb_cases + 1; i++) {
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
        if(plateau->longueur >= 75){
            //printf("test\n");
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
        
        
    }
    //printf("%d\n", plateau->longueur);
    //printf("%d\n", plateau->nb_virages);
}


Direction prochaine_direction(Terrain plateau, Direction derniere_dir){
    int etendu_droite, etendu_gauche;
    int somme = 0;
    switch (derniere_dir) {
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
    int nb = rand() % somme;
    if (nb < etendu_droite){
        switch (derniere_dir){
            case HAUT:
                return DROITE;
            case BAS:
                return GAUCHE;
            case DROITE:
                return BAS;
            case GAUCHE:
                return HAUT;
            default:
                return HAUT;
        }
    }
    switch (derniere_dir){
            case HAUT:
                return GAUCHE;
            case BAS:
                return DROITE;
            case DROITE:
                return HAUT;
            case GAUCHE:
                return BAS;
            default:
                return HAUT;
        }
    
}


int est_case_chemin(Terrain plateau, Case a){
    if(plateau.chemin == NULL){
        return 0;
    }
    for(int i = 0; i < plateau.longueur; i++){
        if(plateau.chemin[i].x == a.x && plateau.chemin[i].y == a.y){
            return 1;
        }
    }
    return 0;
}

void affiche_chemin(Terrain plateau) {
    for (int i = 0; i < LIG; ++i) {
        for (int j = 0; j < COL; ++j) {
            if(plateau.nid.x == i && plateau.nid.y == j ){
                fprintf(stdout, "D ");
            }
            else if (est_case_chemin(plateau, plateau.tab[i][j]) == 1) {
                fprintf(stdout, "C ");
            } else {
                fprintf(stdout, ". ");
            }
        }
        fprintf(stdout, "\n");
    }
}

void test(Terrain jeu){
    for(int i = 0; i < jeu.longueur; i++){
        printf("Case %d suiv = %d\n", i, jeu.chemin[i].suiv);
    }
}
