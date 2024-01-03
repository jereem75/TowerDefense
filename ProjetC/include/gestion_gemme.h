#ifndef GESTION_GE
#define GESTION_GE

#include <stdlib.h>
#include <stdio.h>
#define MAX_GEMMES 1000
#include "gestion_mana.h"
//#include "gestion_tour.h"


typedef enum{
    PYRO,
    DENDRO,
    HYDRO,
    MIXTE
}Type_Gemme;

typedef struct{
    int niveau;
    Type_Gemme type;
    int teinte;
    int temps_restant;
    //int indice_lst;
}Gemme;

typedef struct{
    //Gemme lst_gemmes[MAX_GEMMES];
    //int nb_gemmes;
    Gemme lst_gemmes_file[10];
    int nb_gemmes_file; // nb gemmes max que l'on peut creer = 10
} Liste_gemme;

/* fonction qui initialise la structure Liste_gemme*/
void initialise_gemme(Liste_gemme *lst);

/* fonction qui renvoie la teinte en fonction du type de gemme*/
int def_teinte(Gemme *gem);


/* fonction qui cree une gemme et l'akoute dans les deux listes*/
void cree_gemme(Mana *gestion, Liste_gemme *lst, int n);

/* fonction qui ajoute dans une tour une gemme*/
//void add_gemme_tour(Liste_gemme *lst_gem,Lst_Tour *lst,Gemme *gem, int x, int y);

/* fonction qui decale a gauche les elements de la liste gemmes*/
//void decale_gauche_lst_gemmes(Liste_gemme *lst, int indice);

/* fonction qui decale a gauche les elements de la file gemmes*/
void decale_gauche_lst_gemmes_file(Liste_gemme *lst, Gemme *delete);

/*  fonction qui fusionne une gemme si cela est possible*/
void fusionne(Mana *gestion, Liste_gemme *lst, Gemme *un, Gemme *deux);

#endif
