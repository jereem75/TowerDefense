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
    initialise_cases(&(jeu->plateau));
    jeu->tours.nb_tours = 0;
    jeu->vagues.vague_actuelle = 0;
}

void moteur(Game *jeu) {
    int selected_lvl = 0;
    int statut_t = -1;
    int statut_gem = -1;
    int premiere_vague = 0;
    int val = 0;
    struct timespec endtime, new_time, affichage_old, affichage_new, vague_old, vague_new;
    double frametime, extratime;
    clock_gettime(CLOCK_REALTIME, &affichage_old); 
    
    
    while (1) {
        clock_gettime(CLOCK_REALTIME, &affichage_new); 
        val = clic(jeu, &statut_t, &statut_gem, &selected_lvl);
        if (val == -1 || jeu->mana.nb_mana < 0) {
            break;
        }
        if (val == 1 && premiere_vague == 0) {
            clock_gettime(CLOCK_REALTIME, &vague_old);
            clock_gettime(CLOCK_REALTIME, &vague_new);
            premiere_vague = 1;
            jeu->vagues.vague_actuelle ++;
            initialisation_vague(jeu->plateau
            , &(jeu)->vagues);
            val = 0;
        }
        if (affichage_new.tv_sec - affichage_old.tv_sec >= 0 || affichage_new.tv_nsec - affichage_old.tv_nsec >= 1.0 / 60 * 1e9){
            //printf("test\n");
            update_monster(jeu->plateau, &(jeu)->vagues);
            affichage_old = affichage_new;
            afficher_plateau(&(jeu)->plateau);
            affiche_tours(*jeu, statut_t);
            affiche_monstres(*jeu);
        }

        if(premiere_vague == 1){
            clock_gettime(CLOCK_REALTIME, &vague_new);
            if (vague_new.tv_sec - vague_old.tv_sec > 35 || val == 1) {
                val = 0;
                if(jeu->vagues.vague_actuelle > 0){
                    jeu->mana.nb_mana +=(int) ((double)(vague_old.tv_sec - vague_new.tv_sec + 35) / 100.0 * (double)jeu->mana.nb_max_mana);
                    if(jeu->mana.nb_mana > jeu->mana.nb_max_mana){
                        jeu->mana.nb_mana = jeu->mana.nb_max_mana;
                    }
                }
                jeu->vagues.vague_actuelle ++;
                clock_gettime(CLOCK_REALTIME, &vague_old);
                clock_gettime(CLOCK_REALTIME, &vague_new);
                initialisation_vague(jeu->plateau
            , &(jeu)->vagues);
            }
        }
        
        clock_gettime(CLOCK_REALTIME, &endtime);
        affiche_file_gemme(*jeu, statut_gem);
        affiche_infos();
        affiche_mana(jeu->mana, selected_lvl);
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
}
