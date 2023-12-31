#ifndef GRAPH
#define GRAPH

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#include "moteur.h"
#include <MLV/MLV_all.h>
# define LONGUEUR_CASE 54


/* fonction qui affiche le plateau de jeu */
void afficher_plateau(Terrain *jeu);
/* fonction qui affiche la liste de gemmes */
void affiche_file_gemme(Game jeu, int statut_gemme);

/* fonction qui affiche les infos */
void affiche_infos();

/* fonction qui affiche les infos sur la mana */
void affiche_mana(Mana mana, int lvl_selected);

/* fonction qui efectue des actions en fonction de l'action realisee par l'utilisateur */
int clic(Game *game, int *statut_t, int *statut_gemme, int *lvl_selected);

/* fonction qui convertit un int en couleur */
MLV_Color convert_angle_to_color(int angle);

/* gonction qui renvoie 1 si les coordonnes de la souris sont sur le plateau */
int est_case_grille(int x, int y);

/* fonction qui affiche les tours du jeu */
void affiche_tours(Game jeu, int statut_t);

#endif
