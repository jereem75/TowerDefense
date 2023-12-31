#include <stdio.h>
#include <stdlib.h>
#include "../include/gestion_gemme.h"
#include "../include/gestion_mana.h"
#include "../include/generation_terrain.h"
#include <math.h>


void intialise_gemme(Liste_gemme *lst){
    //lst->nb_gemmes = 0;
    lst->nb_gemmes_file = 0;
    for(int i = 0; i < 10 ; i++ ){
        lst->lst_gemmes_file[i].teinte = -1;
        lst->lst_gemmes_file[i].niveau = -1;
    }
}

int def_teinte(Gemme *gem){
    int n = rand() % 61;
    switch(gem->type){
        case PYRO:
            if(n > 30){
                n += 299;
            }
            return n;
        case DENDRO:
            return n + 90;
        case HYDRO:
            return n + 210;
        default:
            return 0;
    }
}

void cree_gemme(Mana *gestion, Liste_gemme *lst, int n){
    if(lst->nb_gemmes_file < 10){
        int prix = (int) (100 * pow(2.0, (double) n));
        if(prix > gestion->nb_mana){
            return;
        }
        gestion->nb_mana -= prix;
        Gemme new;
        new.niveau = n;
        new.type = rand() % 4;
        new.teinte = def_teinte(&new);
        //new.indice_lst = lst->nb_gemmes;
        //lst->nb_gemmes ++;
        //lst->lst_gemmes_file[new.indice_lst] = new;
        lst->lst_gemmes_file[lst->nb_gemmes_file] = new;
        lst->nb_gemmes_file ++;
    }
}




/*void decale_gauche_lst_gemmes(Liste_gemme *lst, int indice) {

    for (int i = indice; i < lst->nb_gemmes ; i++) {
        lst->lst_gemmes[i] = lst->lst_gemmes[i + 1];
    }

    lst->nb_gemmes--;
}*/


void decale_gauche_lst_gemmes_file(Liste_gemme *lst, Gemme *delete) {
    int test = 0;
    for (int i = 0; i < lst->nb_gemmes_file ; i++) {
        if( &(lst)->lst_gemmes_file[i] == delete || test == 1){
                test = 1;
                lst->lst_gemmes_file[i] = lst->lst_gemmes_file[i + 1];
        }
    }
    lst->lst_gemmes_file[lst->nb_gemmes_file].niveau = -1;
    //printf("teinte = %d\n", lst->lst_gemmes_file[lst->nb_gemmes_file].teinte);
    //printf("taille = %d\n",lst->nb_gemmes_file);
}

void fusionne(Mana *gestion, Liste_gemme *lst, Gemme *un, Gemme *deux){
    if(un == deux){
        fprintf(stderr, "meme gemme\n");
        return;
    }
    if(gestion->nb_mana >= 100){
        if(un->niveau == deux->niveau){
            gestion->nb_mana -=  100;
            if(un->type != deux->type){
                un->type = MIXTE;    
            }
            un->niveau ++;
            un->teinte = (un ->teinte + deux->teinte) / 2;
            deux->niveau = 0;
            deux->teinte = -1;
            lst->nb_gemmes_file --;
            decale_gauche_lst_gemmes_file(lst, deux);
            //decale_gauche_lst_gemmes(lst, deux->indice_lst);
        }
        else{
            fprintf(stderr, "gemme pas de meme niveau\n");
        }
    }
    else{
        fprintf(stderr, "pas assez de mana\n");
    }
}


