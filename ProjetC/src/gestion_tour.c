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
    return -1;
}


void build_tour(Terrain plateau, Mana *gestion, Lst_Tour *lst, int x, int y){
    if( x < 0 || x >= LIG || y < 0 || y >= COL){
        
        fprintf(stderr, "out\n");
        // case pas definie
        return;
    }
    Case tmp;
    tmp.x = x;
    tmp.y = y;
    fprintf(stderr, "%d,%d\n", tmp.x, tmp.y);
    if( est_case_chemin(plateau, tmp) == 1 || est_case_tour(*lst, tmp) > - 1){
        fprintf(stderr, "occupee\n");
        // la case est fait partie du chemin ou est une tour
        return;
    }
    if( lst->nb_tours < 3){
        lst->lst_tours[lst->nb_tours].position = tmp;
        lst->lst_tours[lst->nb_tours].gem.teinte = -1;
        lst->lst_tours[lst->nb_tours].gem.niveau = -1;
        lst->nb_tours ++;
    }
    
    else{
        int prix = 100 * (int)pow(2, lst->nb_tours - 3);
        if(prix <= gestion->nb_mana){
            lst->lst_tours[lst->nb_tours].position = tmp;
            lst->lst_tours[lst->nb_tours].gem.teinte = -1;
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
    int indice = est_case_tour(*lst, tmp);
    if(indice == -1){
        fprintf(stderr, "la case n'est pas une tour");
        return;
    }
    if((lst->lst_tours[indice].gem.teinte) != -1 && lst->lst_tours[indice].gem.niveau != gem->niveau){
        fprintf(stderr, "fusion impossible\n");
        return;
    }
    else if(gem->niveau == lst->lst_tours[indice].gem.niveau){
        lst->lst_tours[indice].gem.niveau ++;
        lst->lst_tours[indice].gem.teinte = (lst->lst_tours[indice].gem.teinte + gem->teinte) / 2;
        lst->lst_tours[indice].gem.type = MIXTE;
    }
    else{
        lst->lst_tours[indice].gem.niveau = gem->niveau;
        lst->lst_tours[indice].gem.teinte = gem->teinte;
        lst->lst_tours[indice].gem.type = gem->type;
    }
    lst->lst_tours[indice].gem.temps_restant = 2;
    lst_gem->nb_gemmes_file --;
    decale_gauche_lst_gemmes_file(lst_gem, gem);
    
}