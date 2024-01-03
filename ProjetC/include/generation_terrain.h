#ifndef GENERATION_TE
#define GENERATION_TE

#include <stdlib.h>
#include <stdio.h>


#define COL 28
#define LIG 22

typedef enum{
    HAUT,
    BAS,
    GAUCHE,
    DROITE
}Direction;

typedef struct{
    int x;
    int y;
    Direction suiv;
}Case;

typedef struct{
    Case tab[LIG][COL];
    Case *chemin;
    Case nid;
    int nb_virages;
    int longueur;
}Terrain;

/* fonction qui initialise les cases du terrain */
void initialise_cases(Terrain *plateau);

/* fonction qui calcule la distance de manhattan entre deux */
int distance_manhattan(Case i, Case j);

/* fonction qui prend une case aleatoire comme nid de mosntres*/
void random_nid(Terrain *plateau);

/* fonction qui affiche le tableau de cases*/
void afficherTableau(Terrain tab);

/* fonction qui calcule l'etendu d'une case dans une direction dir*/
int etendu(Terrain plateau, Case a, Direction dir);

/* fonction qui renvoie 0 si la case est definie et 1 sinon*/
int est_definie(Case a);

/* fonction qui compare une case a et les cases du chemin pour savoir si elle est trop proche*/
int compare_chemin(Case a, Terrain plateau);

/* fonction qui genere le terrain s*/
void generation(Terrain *plateau);

/* fonction qui ajoute nb_cases au chemin cases en paratant d'une case a et en allant dans une direction dir*/
void ajoute_cases_chemin(Terrain *plateau, Case a, Direction dir, int nb_cases);

/* fonction qui calculue la prochaine direction*/
Direction prochaine_direction(Terrain plateau, Direction derniere_dir);

/* fonction qui renvoie 1 si la case a fait partie du chemin*/
int est_case_chemin(Terrain plateau, Case a);

/* fonction qui affiche le chemin dans la grille*/
void affiche_chemin(Terrain plateau);

#endif
