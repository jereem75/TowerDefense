#include <stdio.h>
#include <stdlib.h>
#include "../include/gestion_tour.h"
#include "../include/gestion_mana.h"
#include "../include/gestion_gemme.h"
#include "../include/generation_terrain.h"
#include <math.h>


int est_case_tour(Lst_Tour lst, Case a){
    for(int i = 0; i < lst.nb_tours; i ++){
        if(lst.lst_tours[i].position.x == a.x && lst.lst_tours[i].position.y == a.y){
            return i;
        }
    }
    return 0;
}


void build_tour(Terrain plateau, Mana *gestion, Lst_Tour *lst, int x, int y){
    if( x < 0 || x >= LIG || y < 0 || y >= COL){
        // case pas definie
        return;
    }
    Case tmp;
    tmp.x = x;
    tmp.y = y;
    if( est_case_chemin(plateau, tmp) || est_case_tour(*lst, tmp) > 0){
        // la case est fait partie du chemin
        return;
    }
    if( lst->nb_tours < 3){
        lst->lst_tours[lst->nb_tours].position = tmp;
        lst->nb_tours ++;
    }
    else{
        int prix = 100 * (int)pow(2, lst->nb_tours - 3);
        if(prix <= gestion->nb_mana){
            lst->lst_tours[lst->nb_tours].position = tmp;
            lst->nb_tours ++;
            gestion->nb_mana -= prix;
        }
        else{
            fprintf(stderr, "pas assez de mana\n");
        }
    }
}