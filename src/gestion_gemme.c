#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/gestion_gemme.h"
#include "../include/gestion_mana.h"
#include "../include/generation_terrain.h"



void initialise_gemme(Liste_gemme *lst){
    //on initalise les gemmes
    lst->nb_gemmes_file = 0;
    for(int i = 0; i < 10 ; i++ ){ // on met la teinte et le niveau a -1
        lst->lst_gemmes_file[i].teinte = -1;
        lst->lst_gemmes_file[i].niveau = -1;
    }
}

int def_teinte(Type_Gemme type){ 
    int teinte = rand() % 60;
    if(type == PYRO){
        if(teinte > 30){
            teinte += 230;
        }
    }
    if(type == DENDRO){
        teinte += 90;
    }
    if(type == HYDRO){
        teinte += 210;
    }
    return teinte;
}

void cree_gemme(Mana *gestion, Liste_gemme *lst, int n){
    if(lst->nb_gemmes_file < 10){ // si la file de gemme n'est pas pleine
        int prix = (int) (100 * pow(2.0, (double) n)); // on calcule son prix en fonction du niveau
        if(prix > gestion->nb_mana){
            return;
        }
        gestion->nb_mana -= prix; // on retire l eprix a la mana
        Gemme new;
        new.niveau = n;
        new.type = rand() % 3;
        new.teinte = def_teinte(new.type); // on calcule aleatoirement la teinte
        lst->lst_gemmes_file[lst->nb_gemmes_file] = new;
        lst->nb_gemmes_file ++; // on l'ajoute dans la liste
    }
}




void decale_gauche_lst_gemmes_file(Liste_gemme *lst, Gemme *delete) {
    int test = 0;
    for (int i = 0; i < lst->nb_gemmes_file ; i++) {
        if( &(lst)->lst_gemmes_file[i] == delete || test == 1){ // si c'est la gemme que l'on veut supprimer
                test = 1;
                lst->lst_gemmes_file[i] = lst->lst_gemmes_file[i + 1];
        }
    }// on decale a gauche
    lst->lst_gemmes_file[lst->nb_gemmes_file].niveau = -1;
    // on met la teinte de la gemme a -1 pour pas l'afficher
}

void fusionne(Mana *gestion, Liste_gemme *lst, Gemme *un, Gemme *deux){
    if(un == deux){ // si c'est la meme gemme
        fprintf(stderr, "meme gemme\n");
        return;
    }
    if(gestion->nb_mana >= 100){ // si on a assez de mana
        if(un->niveau == deux->niveau){
            gestion->nb_mana -=  100;
            if(un->type != deux->type){ // si elles ne sont pas de meme type
                un->type = MIXTE;    
            }
            un->niveau ++;
            un->teinte = (un ->teinte + deux->teinte) / 2; // on calcule la nouvelle teinte
            deux->niveau = 0;
            deux->teinte = -1;
            lst->nb_gemmes_file --;
            decale_gauche_lst_gemmes_file(lst, deux); // on decale la file de gemme
        }
        else{
            fprintf(stderr, "gemme pas de meme niveau\n");
        }
    }
    else{
        fprintf(stderr, "pas assez de mana\n");
    }
}


