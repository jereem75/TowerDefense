#include <stdio.h>
#include <stdlib.h>
#include "../include/generation_terrain.h"
#include "../include/gestion_mana.h"
#include <time.h>
#include <MLV/MLV_all.h>
#include "../include/graphique.h"
#include "../include/moteur.h"

int main(){
    MLV_create_window("TowerDefense", "TowerD", 1512, 1500);
    srand(time(NULL));
    Game jeu;
    initialise(&jeu);
    //afficherTableau(test);
    generation(&(jeu).plateau);
    printf("main\n");
    //affiche_chemin(jeu.plateau);
    afficher_plateau(&(jeu).plateau);
    //printf("longuueur = %d, virage = %d\n", test.longueur, test.nb_virages);
    moteur(&jeu);
    //MLV_wait_seconds(5);
    MLV_free_window();
    return 0;
}