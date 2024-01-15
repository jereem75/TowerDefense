#ifndef GESTION_TI
#define GESTION_TI

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#include "gestion_monstre.h"
#include "gestion_gemme.h"
#define D 8

typedef struct{
    double x;
    double y;
    int indice_vague;
    int indice_monstre;
    int nb_tirs; 
}Tir;

/* fontion qui initalise la structure tir en fonction des positions de la tour */
void initialisation_tir(Tir *tir, int x_tour, int y_tour);

/* fonction qui met a jour la manan et le tir en fonction des positions des monstres  */
void update_tir(Mana *mana, Tir *tir, Lst_Vague *vague, Gemme gemme, int tour_x, int tour_y, int longueur_chemin); 

/* fonction qui renvoie 1 si un mosntre est dans le rayon 0 sinon*/
int est_dans_rayon(Tir tir, int x_monstre, int y_monstre);

/* fonction qui renvoie les degats du tir en fonction de la teinte de la gemme et la teainte du monstre */
int degats(Gemme gemme, Tir tir, int teinte_monstre);

/* fonction qui calcule la distance entre le monstre et le tir*/
double calcul_distance_monstre_tir(Tir tir, double x_monstre, double y_monstre);

/* fonction qui calcule le vecteur unitaire  MT(monstre - tir) */
double calcul_vecteur_unitaire(Tir tir, double monstre_coord, double monstre_x, double monstre_y, double tir_coord);

/* fonction qui met a jour la mana, le tir, le monstre en fonction des degats affectes au monstre*/
void frappe_monstre(Tir *tir, Gemme gemme, Lst_Vague *vague, Monstre *monstre, Mana *mana);

/* fonction qui applique l'effet pyro aux monstres situe dans le rayon du monstre frappe par l'effet pyro*/
void applique_pyro_monstres(Gemme gemme, Tir tir, Lst_Vague *vague, Monstre monstre);

/* fonction qui applique l'effet pyro hydro au monstre impacte*/
void applique_pyro_hydro(Gemme gemme, Tir tir, Lst_Vague *vague, Monstre monstre);

/* fonction qui trouve une cible pour un tir en parcourant la liste des vagues de monstres*/
void trouve_cible(Lst_Vague *vague, Tir *tir);

#endif
