#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "../include/moteur.h"
#include "../include/gestion_gemme.h"
#include "../include/generation_terrain.h"
#include "../include/graphique.h"
#include <MLV/MLV_all.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>


void initialise(Game *jeu) {
    initialisation_mana(&(jeu->mana));
    initialise_gemme(&(jeu->gemmes));
    //initialise_cases(&(jeu->plateau));
    jeu->tours.nb_tours = 0;
    jeu->vagues.vague_actuelle = 0;
    jeu->vagues.total_degats = 0;
}

void moteur(Game *jeu) {
    int selected_lvl = 0; // niveau de gemme selectionne
    int statut_t = -1; // statut touche T
    int statut_gem = -1; // statut gemme selectionnee
    int premiere_vague = 0; // 1 si premiere vague effectuee 0 sinon
    int val = 0; // valeur de retour de la fonction clic
    struct timespec endtime, new_time, vague_old, vague_new;
    double frametime, extratime;
    while (val != -1 && jeu->mana.nb_mana >= 0 && MAX_VAGUES - 5 != jeu->vagues.vague_actuelle) {
        val = clic(jeu, &statut_t, &statut_gem, &selected_lvl);
        // on effectue le clic et on recupere les valeurs en fonction de l'action faite
        if (val == 1 && premiere_vague == 0) { // si on lance la premiere vague
            // on lance les chronos pour les prochaines vagues
            clock_gettime(CLOCK_REALTIME, &vague_old);
            clock_gettime(CLOCK_REALTIME, &vague_new);
            premiere_vague = 1;
            jeu->vagues.vague_actuelle ++;
            initialisation_vague(jeu->plateau, &(jeu)->vagues); // on initialise la vague
            val = 0;
        }
        // on raffraichit les affichages
        update_monster(jeu->plateau, &(jeu)->vagues, &(jeu)->mana);
        udpate_tours(&(jeu)->mana, &(jeu)->tours, &(jeu)->vagues, jeu->plateau.longueur);
        afficher_plateau(&(jeu)->plateau);
        affiche_tours(*jeu, statut_t);
        affiche_monstres(*jeu);
        affiche_file_gemme(*jeu, statut_gem);
        affiche_infos();
        affiche_mana(jeu->mana, selected_lvl);
        if(premiere_vague == 1){
            clock_gettime(CLOCK_REALTIME, &vague_new);
            if (vague_new.tv_sec - vague_old.tv_sec > 35 || val == 1) {
                if(jeu->vagues.vague_actuelle > 0 && val == 1){
                    // si ll'on clique avant la fin du chrono on lance la vague et on recupere une partie de mana
                    jeu->mana.nb_mana +=(int) ((double)(vague_old.tv_sec - vague_new.tv_sec + 35) / 100.0 * (double)jeu->mana.nb_max_mana);
                    if(jeu->mana.nb_mana > jeu->mana.nb_max_mana){
                        jeu->mana.nb_mana = jeu->mana.nb_max_mana;
                    }
                }
                val = 0;
                jeu->vagues.vague_actuelle ++;
                clock_gettime(CLOCK_REALTIME, &vague_old);
                clock_gettime(CLOCK_REALTIME, &vague_new);
                initialisation_vague(jeu->plateau, &(jeu)->vagues);
                // on intinialise la prochaine vague
            }
        }
        // on affiche les informations
        clock_gettime(CLOCK_REALTIME, &endtime);
        info_vague(*jeu, vague_old.tv_sec - vague_new.tv_sec + 35) ;
        MLV_actualise_window();
        clock_gettime(CLOCK_REALTIME, &new_time);
        frametime = new_time.tv_sec - endtime.tv_sec;
        frametime += (new_time.tv_nsec - endtime.tv_nsec) / 1.0E9;
        extratime = 1.0 / 60 - frametime;
        if (extratime > 0) {
            MLV_wait_milliseconds((int)(extratime * 1000));
        }
    }
    free(jeu->plateau.chemin);
    jeu->plateau.chemin = NULL;
}
