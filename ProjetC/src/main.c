#include <stdio.h>
#include <stdlib.h>
#include "../include/generation_terrain.h"
#include "../include/gestion_mana.h"
#include <time.h>
#include <MLV/MLV_all.h>
#include "../include/graphique.h"
#include "../include/moteur.h"



int main(){
    MLV_create_window("TowerDefense", "TowerD", COL * LONGUEUR_CASE, LIG * LONGUEUR_CASE + 200); // on cree la fenetre graphique
    srand(time(NULL));
    Game jeu;
    initialise(&jeu);
    generation(&(jeu).plateau); // on genere le chemin
    afficher_plateau(&(jeu).plateau); // on affiche le plateau
    moteur(&jeu); // on lance le jeu
    affiche_resultats(jeu);
    MLV_free_window();
    return 0;
}
