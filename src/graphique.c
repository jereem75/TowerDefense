#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <math.h>

#include "../include/graphique.h"
#include "../include/generation_terrain.h"
#include "../include/gestion_gemme.h"
#include "../include/gestion_tour.h"



void afficher_plateau(Terrain *jeu) {
    int i, j;
    for (i = 0; i < LIG; i++) {
        for (j = 0; j < COL; j++) {
            MLV_draw_filled_rectangle(j* LONGUEUR_CASE, i * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_GREEN4);
            MLV_draw_rectangle(j * LONGUEUR_CASE, i * LONGUEUR_CASE, LONGUEUR_CASE, LONGUEUR_CASE, MLV_COLOR_BLACK);
        }
    }
    for(int i = 0; i < jeu->longueur; i++){
        MLV_draw_filled_rectangle(jeu->chemin[i].y * LONGUEUR_CASE, jeu->chemin[i].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_GREY);
        MLV_draw_rectangle(jeu->chemin[i].y * LONGUEUR_CASE, jeu->chemin[i].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_BLACK);
    }
    // on affiche le nid et le camp
    MLV_draw_filled_rectangle(jeu->chemin[0].y * LONGUEUR_CASE, jeu->chemin[0].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_PURPLE1);
    MLV_draw_filled_rectangle(jeu->chemin[jeu->longueur - 1].y * LONGUEUR_CASE, jeu->chemin[jeu->longueur - 1].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_RED);

}


void affiche_file_gemme(Game jeu, int statut_gemme){
    int length = jeu.gemmes.nb_gemmes_file;
    for(int i = 0; i < 10; i++){
        MLV_draw_filled_rectangle(i * 52, 835, 52, 52, MLV_COLOR_WHITE);
        MLV_draw_rectangle(i * 52, 835, 52, 52, MLV_COLOR_BLACK);
        if (i < length) {
            MLV_draw_filled_circle(i * 52 + 26, 861, 24, convert_angle_to_color(jeu.gemmes.lst_gemmes_file[i].teinte));
            MLV_draw_text_box(i * 52, 887, 52, 15,"LVL %d",1, MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, jeu.gemmes.lst_gemmes_file[i].niveau);
            MLV_draw_text_box(i * 52, 902, 52, 15,"%s",1, MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, affiche_effet_reduit(jeu.gemmes.lst_gemmes_file[i].type));
        }
        else{
            MLV_draw_text_box(i * 52, 887, 52, 32,"",1, MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        }
    }
    if( statut_gemme == -1){
        MLV_draw_text_box(0,796, 800, 20,"APPUYEZ SUR UNE GEMME DE LA LISTE POUR FUSIONNER", 25, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    }
    else{
        MLV_draw_text_box(0,796, 800, 20,"GEMME N°%d CHOISIE APPYEZ SUR UNE TOUR OU UNE GEMME DE MEME NIVEAU POUR FUSIONNER(100 MANA)", 25, MLV_COLOR_BLACK, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, statut_gemme + 1);

    }

}





void affiche_infos(){
    MLV_draw_adapted_text_box(0,815,"Gemmes générées:", 1, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER);
    MLV_draw_text_box(475,920,75,24, "UP LVL",1,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    MLV_draw_text_box(475,945,75,24, "DOWN LVL",1,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);

}

void affiche_mana(Mana mana, int lvl_selected){
    int prix = (int) (100 * pow(2.0, (double) lvl_selected));
    MLV_draw_adapted_text_box(0,780,"MANA: %d/%d", 1, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, mana.nb_mana, mana.nb_max_mana);
    MLV_draw_text_box(550,865,430,50, "AUGMENTER LA RESERVE VERS NIVEAU %d: %d MANA",1, MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, mana.niveau + 1, (int)(500 * pow(1.4, mana.niveau)) );
    MLV_draw_text_box(550,920,430,50,"GENERER UNE GEMME DE NIVEAU %d POUR %d MANA",1,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, lvl_selected, prix);
}


int clic(Game *game, int *statut_t, int *statut_gemme, int *lvl_selected) {
    MLV_Event event;
    MLV_Keyboard_button key_sym;
    MLV_Mouse_button mouse_button;
    int x, y;
    MLV_Button_state state;
    event = MLV_get_event(&key_sym, NULL, NULL, NULL, NULL, &x, &y, &mouse_button, &state);
    if (event == MLV_KEY) { // si l'evenement est une touche
        if (key_sym == MLV_KEYBOARD_q) { // si l'on appuie sur la touche q on quitte
            return -1;
        } else if (key_sym == MLV_KEYBOARD_t) { // si l'on appuie sur la touche T on active ou desactive la creation de tour
            *statut_t *= -1;
        }
    }
    else if (event == MLV_MOUSE_BUTTON) {
        if (state == MLV_RELEASED) {
            if (x > 550 && x < 980 && y > 920 && y < 970) { // si l'on clique sur le bouton creer gemme
                cree_gemme(&(game)->mana, &(game)->gemmes, *lvl_selected);
                return 0;
            }
            else if(x > 830 && x < 980 && y > 770 && y < 820){ // si l'on clique sur lancer vague
                return 1;
            }
            else if(x > 550 && x < 980 && y > 865 && y < 915){ // si l'on clique sur augmenter mana
                up_lvl_mana( &(game)->mana);
            }
            else if(x > 475 && x < 550 && y > 920 && y < 944){ // si l'on clique sur augmenter le niveau de la gemme
                (*lvl_selected) ++;
            }
            else if(x > 475 && x < 550 && y > 945 && y < 969){// si l'on clique sur reduire le niveau de la gemme
                if(*lvl_selected != 0){ // si c'est pas deja 0
                    (*lvl_selected) --;
                }
            }
            else if( x < 520  && y > 835 && y < 887){ // si l'on clique sur la file de gemme
                if(*statut_gemme == -1){ // si l'on n'avait pas deja cliquer sur une gemme
                    *statut_gemme = x / 52; // on recupere l'indice de la gemme
                    if(*statut_gemme >= game->gemmes.nb_gemmes_file){ // si l'indice ne correpond pas a une gemme disponible
                        *statut_gemme = -1;
                    }
                }
                else{ // sinon on fusionne avec la gemme clique
                    fusionne(&(game)->mana, &(game)->gemmes,&(game)->gemmes.lst_gemmes_file[*statut_gemme],&(game)->gemmes.lst_gemmes_file[x / 52 ]);
                    *statut_gemme = -1;
                    return 0;
                }
            }
            else if(*statut_gemme > -1){
                if(est_case_grille(x,y) == 0 ){ // si le clic est sur la fenetre
                    // on ajoute la gemme dans la tour si cela est possible
                    add_gemme_tour(&(game->gemmes), &(game)->tours, &(game)->gemmes.lst_gemmes_file[*statut_gemme], y / LONGUEUR_CASE, x / LONGUEUR_CASE );
                    *statut_gemme = -1;
                }
            }
            else if (*statut_t == 1) { // si la touche T est pressee
                if (est_case_grille(x, y) != 1){ // si le clic est dans la grille
                    // on construit la tour
                    build_tour(game->plateau, &(game)->mana, &(game)->tours, y/LONGUEUR_CASE, x/LONGUEUR_CASE);
                }
            }
        }
    }
    return 0;
}


MLV_Color convert_angle_to_color(int angle) {
    float s = 1.0;
    float l = 0.5;
    float chroma = (1 - fabs(2 * l - 1)) * s;
    float h_prime = angle / 360.0; 
    float x = chroma * (1 - fabs(fmod(h_prime * 6, 2) - 1));
    float m = l - chroma / 2;

    float r, g, b;

    if (h_prime < 1.0 / 6.0) {
        r = chroma;
        g = x;
        b = 0;
    } else if (h_prime < 2.0 / 6.0) {
        r = x;
        g = chroma;
        b = 0;
    } else if (h_prime < 3.0 / 6.0) {
        r = 0;
        g = chroma;
        b = x;
    } else if (h_prime < 4.0 / 6.0) {
        r = 0;
        g = x;
        b = chroma;
    } else if (h_prime < 5.0 / 6.0) {
        r = x;
        g = 0;
        b = chroma;
    } else {
        r = chroma;
        g = 0;
        b = x;
    }
    return MLV_convert_rgba_to_color((r + m) * 255, (g + m)* 255, (b + m)* 255, 255);
}


int est_case_grille(int x, int y){ // si la case est dans la grile
    if (x > 28 * LONGUEUR_CASE || y > 22 * LONGUEUR_CASE){
        return 1;
    }
    return 0;
}


void affiche_tours(Game jeu, int statut_t ){
    int n = jeu.tours.nb_tours;
    for(int i = 0; i < n; i++){
        if(jeu.tours.lst_tours[i].gem.teinte != -1){ // si la gemme n'est pas vide
            MLV_draw_filled_circle(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 2, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 2, LONGUEUR_CASE / 2, convert_angle_to_color(jeu.tours.lst_tours[i].gem.teinte ));
            MLV_draw_text(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 3 - 10, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 6, "LVL %d", MLV_COLOR_BLACK, jeu.tours.lst_tours[i].gem.niveau);
            MLV_draw_text(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 3 - 7, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 2 + 1, "%s", MLV_COLOR_BLACK, affiche_effet_reduit(jeu.tours.lst_tours[i].gem.type));
            if(statut_t ==1){
                MLV_draw_circle(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 2, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 2, 3 * LONGUEUR_CASE, convert_angle_to_color(jeu.tours.lst_tours[i].gem.teinte ));
            }
            if(jeu.tours.lst_tours[i].tir.indice_monstre != -1 || jeu.tours.lst_tours[i].tir.indice_vague != -1){
                MLV_draw_filled_circle(jeu.tours.lst_tours[i].tir.y * LONGUEUR_CASE + LONGUEUR_CASE / 2, jeu.tours.lst_tours[i].tir.x * LONGUEUR_CASE + LONGUEUR_CASE / 2 , 5, MLV_COLOR_BLACK );
            }
        }
        else{
            MLV_draw_filled_circle(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 2, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 2, LONGUEUR_CASE / 2, MLV_COLOR_GREY);
        }
    }
    int  prix = 100 * (int)pow(2, jeu.tours.nb_tours - 3);
    if( statut_t == -1){
        MLV_draw_text_box(100,775, 750, 20,"MAINTENEZ LA TOUCHE T POUR CONSTRUIRE UNE TOUR POUR %d DE MANA", 1, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, prix);
    }
    else{
        MLV_draw_text_box(100,775, 750, 20,"EN MODE CONSTRUCTION PRIX DE LA TOUR %d (REALCHEZ T POUR ARRETER DE CONSTRUIRE)", 1, MLV_COLOR_BLACK, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, prix);

    }

}


void affiche_monstres(Game jeu){
    int n = jeu.vagues.vague_actuelle + 1;
    for(int i = 0; i < n; i ++){
        int nb = jeu.vagues.tab[i].nb_monstres;
        for (int j = 0; j < nb; j++){
            // si le monstre est das le nid
            if(jeu.vagues.tab[i].monstres[j].indice_case_actuelle == jeu.plateau.longueur - 1 ){
                continue;
            }
            if(jeu.vagues.tab[i].monstres[j].indice_case_actuelle != 0 && jeu.vagues.tab[i].monstres[j].hp_restants > 0){
                MLV_draw_filled_circle(jeu.vagues.tab[i].monstres[j].y * LONGUEUR_CASE + CENTRE_CASE, jeu.vagues.tab[i].monstres[j].x  * LONGUEUR_CASE + CENTRE_CASE, 15, convert_angle_to_color(jeu.vagues.tab[i].monstres[j].teinte));
                char* type= affiche_type_monstre(jeu.vagues.tab[i]);
                char * effet =affiche_effet(jeu.vagues.tab[i].monstres[j].effet);
                MLV_draw_text(jeu.vagues.tab[i].monstres[j].y * LONGUEUR_CASE + CENTRE_CASE - 14, jeu.vagues.tab[i].monstres[j].x  * LONGUEUR_CASE + CENTRE_CASE - 8,"%s", MLV_COLOR_BLACK, type);
                MLV_draw_text(jeu.vagues.tab[i].monstres[j].y * LONGUEUR_CASE, jeu.vagues.tab[i].monstres[j].x  * LONGUEUR_CASE - 15,"%s", MLV_COLOR_BLACK, effet);
                MLV_draw_filled_rectangle(jeu.vagues.tab[i].monstres[j].y * LONGUEUR_CASE + 10, jeu.vagues.tab[i].monstres[j].x  * LONGUEUR_CASE,35, 10, MLV_COLOR_RED);
                MLV_draw_filled_rectangle(jeu.vagues.tab[i].monstres[j].y * LONGUEUR_CASE + 10, jeu.vagues.tab[i].monstres[j].x  * LONGUEUR_CASE,35 * (jeu.vagues.tab[i].monstres[j].hp_restants /(double) jeu.vagues.tab[i].monstres[j].hp_initial ), 10, MLV_COLOR_GREEN1);
            }

        }
    }
}


void info_vague(Game jeu, long int temps){
    MLV_draw_text_box(820,770,160,50, "LANCER LA VAGUE %d",1,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, jeu.vagues.vague_actuelle + 1);
    if(jeu.vagues.vague_actuelle > 0){
        MLV_draw_text_box(0,950, 450, 15, "VAGUE N°%d, TEMPS RESTANT AVANT LA PROCHAINE VAGUE %ld",1,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, jeu.vagues.vague_actuelle, temps);

    }
}

char* affiche_type_monstre(Vague monster){
    switch(monster.type_vague){
        case NORMAL:
            return "NOR";
        case FOULE:
            return "FOU";
        case AGILE:
            return "AGI";
        case BOSS:
            return "BOS";
    }
    return "NULL";
}



char* affiche_effet(Type_Gemme effet){
    switch(effet){
        case HYDRO:
            return "HYDRO";
        case DENDRO:
            return "DENDRO";
        case PYRO:
            return "PYRO";
        case PYRO_DENDRO:
            return "PYRO_DENDRO";
        case PYRO_HYDRO:
            return "PYRO_HYDRO";
        case DENDRO_HYDRO:
            return "DENDRO_HYDRO";
        case MIXTE:
            return "MIXTE";
        default:
            break;
    }
    return "";
}

char* affiche_effet_reduit(Type_Gemme effet){
    switch(effet){
        case HYDRO:
            return "HYD";
        case DENDRO:
            return "DEN";
        case PYRO:
            return "PYR";
        case MIXTE:
            return "MIX";
        default:
            break;
    }
    return "";
}

void affiche_resultats(Game jeu) {
    MLV_clear_window(MLV_COLOR_WHITE);
    MLV_draw_text(300, 300, "PARTIE TERMINEE !!!, NB VAGUES %d, DEGATS TOTAL %d", MLV_COLOR_BLACK, jeu.vagues.vague_actuelle, jeu.vagues.total_degats);
    MLV_actualise_window();
    MLV_wait_seconds(2);
}
