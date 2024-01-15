#ifndef GRAPH
#define GRAPH

#include <stdlib.h>
#include <stdio.h>
#include "generation_terrain.h"
#include "moteur.h"
#include <MLV/MLV_all.h>
#define LONGUEUR_CASE 35
#define CENTRE_CASE LONGUEUR_CASE / 2


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

/* fonction qui affiche les infos des vagues graphiquement*/
void info_vague(Game jeu, long int temps);

/* fonction qui affiche les monstres graphiquement*/
void affiche_monstres(Game jeu);

/* fonction qui prend une vague et renvoie le type du monstre sous forme de chaine de caracteres afin
 de l'afficher graphiquement */
char* affiche_type_monstre(Vague monster);

/* fonction qui prend un effet et renvoie son effet en chaine de caracteres afin de l'afficher*/
char* affiche_effet(Type_Gemme effet);

/* fonction qui affiche le resultat de la partie */
void affiche_resultats(Game jeu);

/* fonction qui prend un effet et renvoie son effet en chaine de caracteres afin d'affiche les 3 premieres lettresr*/
char* affiche_effet_reduit(Type_Gemme effet);

#endif
