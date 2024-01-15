#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/gestion_tour.h"
#include "../include/gestion_mana.h"
#include "../include/gestion_gemme.h"
#include "../include/generation_terrain.h"
//#include "../include/gestion_monstre.h"
#include <math.h>


int est_case_tour(Lst_Tour lst, Case a){
    for(int i = 0; i < lst.nb_tours; i ++){ // on parcourt toutes les tours
        if(lst.lst_tours[i].position.x == a.x && lst.lst_tours[i].position.y == a.y){ // si la case est une tour on renvoie l'indice
            return i;
        }
    }
    return -1;
}


void build_tour(Terrain plateau, Mana *gestion, Lst_Tour *lst, int x, int y){
    if( x < 0 || x >= LIG || y < 0 || y >= COL){
        // on verifie que la tour est bien dans la grille
        fprintf(stderr, "out\n");
        // case pas definie
        return;
    }
    Case tmp;
    tmp.x = x;
    tmp.y = y;
    //fprintf(stderr, "%d,%d\n", tmp.x, tmp.y);
    if( est_case_chemin(plateau, tmp) == 1 || est_case_tour(*lst, tmp) > - 1){
        fprintf(stderr, "occupee\n");
        // la case  fait partie du chemin ou est une tour
        return;
    }
    if( lst->nb_tours < 3){ // si c'est une des trois premieres elle est gratuite
        lst->lst_tours[lst->nb_tours].position = tmp;
        lst->lst_tours[lst->nb_tours].gem.teinte = -1; // on met la teinte et le niveau a -1 car pas de gemme
        lst->lst_tours[lst->nb_tours].gem.niveau = -1;
        initialisation_tir(&(lst)->lst_tours[lst->nb_tours].tir, lst->lst_tours[lst->nb_tours].position.x, lst->lst_tours[lst->nb_tours].position.y );
        lst->nb_tours ++;
    }
    
    else{
        int prix = 100 * (int)pow(2, lst->nb_tours - 3); // sinon elle est payante
        if(prix <= gestion->nb_mana){
            lst->lst_tours[lst->nb_tours].position = tmp;
            lst->lst_tours[lst->nb_tours].gem.teinte = -1;
            lst->lst_tours[lst->nb_tours].gem.niveau = -1;
            lst->nb_tours ++;
            gestion->nb_mana -= prix;
        }
        else{
            fprintf(stderr, "pas assez de mana\n");
        }
    }
}

void add_gemme_tour(Liste_gemme *lst_gem, Lst_Tour *lst,Gemme *gem, int x, int y){
    Case tmp;
    tmp.x = x;
    tmp.y = y;
    int indice = est_case_tour(*lst, tmp); // on verifie que la case est une tour
    if(indice == -1){
        fprintf(stderr, "la case n'est pas une tour");
        return;
    }
    if((lst->lst_tours[indice].gem.teinte) != -1 && lst->lst_tours[indice].gem.niveau != gem->niveau){
        // on verifie qu'elles sont de meme niveau
        fprintf(stderr, "fusion impossible\n");
        return;
    }
    else if(lst->lst_tours[indice].gem.niveau != -1){ // si la tour a deja une gemme
        lst->lst_tours[indice].gem.niveau ++;
        lst->lst_tours[indice].gem.teinte = (lst->lst_tours[indice].gem.teinte + gem->teinte) / 2;
        if(gem->type != lst->lst_tours[indice].gem.type){ // si les deux gemmes ne sont pas de meme type alors type resultant est MIXTE
            lst->lst_tours[indice].gem.type = MIXTE;
        }
        
    }
    else{ // sinon on met la gemme dans la tour
        lst->lst_tours[indice].gem.niveau = gem->niveau;
        lst->lst_tours[indice].gem.teinte = gem->teinte;
        lst->lst_tours[indice].gem.type = gem->type;
    }
    
    clock_gettime(CLOCK_REALTIME, &(lst->lst_tours[indice].debut_gem));
    // on lance le chrono de chargement de la gemme
    lst_gem->nb_gemmes_file --;
    // on decale les gemmes
    decale_gauche_lst_gemmes_file(lst_gem, gem);
    // on initialise le tir
    initialisation_tir(&(lst)->lst_tours[indice].tir, x, y);
}

void udpate_tours(Mana *mana, Lst_Tour *lst, Lst_Vague *vague, int longeur_chemin){
    for(int i = 0; i <lst->nb_tours; i++){ // on parcourt les tours
        if(lst->lst_tours[i].gem.teinte == -1){ // si il n'y a pas de gemmes 
            continue; // on ignore
        }
        clock_gettime(CLOCK_REALTIME, &(lst->lst_tours[i].fin_gem));
        // on regarde si la gemme est chargee
        if(lst->lst_tours[i].fin_gem.tv_sec - lst->lst_tours[i].debut_gem.tv_sec >= 2){
            if(lst->lst_tours[i].tir.nb_tirs == 0){ //si  l'on a pas fait de tirs
                clock_gettime(CLOCK_REALTIME, &(lst->lst_tours[i].debut_tir));
                // on lance le chrono
            }
            clock_gettime(CLOCK_REALTIME, &(lst->lst_tours[i].fin_tir));
            if(lst->lst_tours[i].fin_tir.tv_sec - lst->lst_tours[i].debut_tir.tv_sec >= 1){
                // on renitialis si on a depasse 1 sec
                lst->lst_tours[i].tir.nb_tirs = 0;
                clock_gettime(CLOCK_REALTIME, &(lst->lst_tours[i].debut_tir));
            }
            // on met a jour les tirs
            update_tir(mana, &(lst)->lst_tours[i].tir, vague, lst->lst_tours[i].gem, lst->lst_tours[i].position.x, lst->lst_tours[i].position.y, longeur_chemin);
        }
    }
}