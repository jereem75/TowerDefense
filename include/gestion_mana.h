#ifndef GESTION_MA
#define GESTION_MA

#include <stdlib.h>
#include <stdio.h>


typedef struct{
    int niveau;
    int nb_mana;
    int nb_max_mana; 
}Mana;


/* foction qui initialise la structure mana*/
void initialisation_mana( Mana *mana);

/* fonction qui ameliore le niveau de la mana si cela est possible */
void up_lvl_mana( Mana *mana);

/* fonction qui gere le niveau de la mana en fonction du monstre tue*/
void gain_mana(Mana *mana, int hp_initial);

/* fonction qui calcule la mana perdue en fonction du monstre arrivant au camp du joueur*/
void perte_mana(Mana *mana, int hp_initial);

#endif
