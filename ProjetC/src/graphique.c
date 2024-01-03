#include <stdio.h>
#include <stdlib.h>
#include "../include/graphique.h"
#include "../include/generation_terrain.h"
#include "../include/gestion_gemme.h"
#include "../include/gestion_tour.h"
#include <MLV/MLV_all.h>
#include <math.h>


void afficher_plateau(Terrain *jeu) {
    //printf("test\n");
    int i, j;
    for (i = 0; i < LIG; i++) {
        for (j = 0; j < COL; j++) {
            MLV_draw_filled_rectangle(j* LONGUEUR_CASE, i * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_GREEN4);
            MLV_draw_rectangle(j * LONGUEUR_CASE, i * LONGUEUR_CASE, LONGUEUR_CASE, LONGUEUR_CASE, MLV_COLOR_BLACK);
        }
    }
    for(int i = 0; i < jeu->longueur; i++){
        MLV_draw_filled_rectangle(jeu->chemin[i].y * LONGUEUR_CASE, jeu->chemin[i].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_YELLOW1);
        MLV_draw_rectangle(jeu->chemin[i].y * LONGUEUR_CASE, jeu->chemin[i].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_BLACK);
    }
    MLV_draw_filled_rectangle(jeu->chemin[0].y * LONGUEUR_CASE, jeu->chemin[0].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_PURPLE1);
    MLV_draw_filled_rectangle(jeu->chemin[jeu->longueur - 1].y * LONGUEUR_CASE, jeu->chemin[jeu->longueur - 1].x * LONGUEUR_CASE, LONGUEUR_CASE , LONGUEUR_CASE, MLV_COLOR_RED);
    
}


void affiche_file_gemme(Game jeu, int statut_gemme){
    int length = jeu.gemmes.nb_gemmes_file;
    for(int i = 0; i < 10; i++){
        MLV_draw_filled_rectangle(i * 80, 1320, 80, 80, MLV_COLOR_WHITE);
        MLV_draw_rectangle(i * 80, 1320, 80, 80, MLV_COLOR_BLACK); 
        if (i < length) {
            MLV_draw_filled_circle(i * 80 + 40, 1360, 20, convert_angle_to_color(jeu.gemmes.lst_gemmes_file[i].teinte));
            MLV_draw_text_box(i * 80, 1400, 80, 80,"LVL %d",25, MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, jeu.gemmes.lst_gemmes_file[i].niveau);
        }
        else{
            MLV_draw_text_box(i * 80, 1400, 80, 80,"",25, MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        }
    }
    if( statut_gemme == -1){
        MLV_draw_text_box(200,1250, 800, 20,"APPUYEZ SUR UNE GEMME DE LA LISTE POUR FUSIONNER", 25, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    }
    else{
        MLV_draw_text_box(200,1250, 800, 20,"GEMME N°%d CHOISIE APPYEZ SUR UNE TOUR OU UNE GEMME DE MEME NIVEAU POUR FUSIONNER(100 MANA)", 25, MLV_COLOR_BLACK, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, statut_gemme + 1);

    }

}





void affiche_infos(){
    MLV_draw_adapted_text_box(0,1255,"Gemmes générées:", 25, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER);
    MLV_draw_text_box(1000,1190,90,40, "UP LVL",25,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    MLV_draw_text_box(1000,1240,90,40, "DOWN LVL",25,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);

}

void affiche_mana(Mana mana, int lvl_selected){
    int prix = (int) (100 * pow(2.0, (double) lvl_selected));
    MLV_draw_adapted_text_box(0,1190,"MANA: %d/%d", 25, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, mana.nb_mana, mana.nb_max_mana);
    MLV_draw_text_box(1100, 1300, 412, 100, "AUGMENTER LA RESERVE VERS NIVEAU %d: %d MANA",25, MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, mana.niveau + 1, (int)(500 * pow(1.4, mana.niveau)) );
    MLV_draw_text_box(1100,1190,412,100,"GENERER UNE GEMME DE NIVEAU %d POUR %d MANA",25,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, lvl_selected, prix);
}


int clic(Game *game, int *statut_t, int *statut_gemme, int *lvl_selected) {
    MLV_Event event;
    MLV_Keyboard_button key_sym;
    MLV_Mouse_button mouse_button;
    int unicode;
    char *texte;
    MLV_Input_box *input_box;
    int x, y;
    MLV_Button_state state;
    event = MLV_get_event(&key_sym, NULL, &unicode, &texte, &input_box, &x, &y, &mouse_button, &state);
    if (event == MLV_MOUSE_BUTTON) {
        if (state == MLV_RELEASED) {
            if (x > 1100 && x < 1512 && y > 1190 && y < 1290) {
                cree_gemme(&(game)->mana, &(game)->gemmes, *lvl_selected);
                return 0;
            }
            else if(x > 1100 && x < 1512 && y > 1410 && x < 1485){
                game->vagues.vague_actuelle ++;
                return 1;
            }
            else if(x > 1100 && x < 1512 && y > 1300 && y < 1400){
                up_lvl_mana( &(game)->mana);
            }
            else if(x > 1000 && x < 1090 && y > 1190 && y < 1230){
                (*lvl_selected) ++;
            }
            else if(x > 1000 && x < 1090 && y > 1240 && y < 1280){
                if(*lvl_selected != 0){
                    (*lvl_selected) --;
                }
            }
            else if( x < 800  && y > 1320 && y < 1400){
                if(*statut_gemme == -1){
                    *statut_gemme = x / 80;
                    if(*statut_gemme >= game->gemmes.nb_gemmes_file){
                        *statut_gemme = -1;
                    }
                }
                else{
                    fusionne(&(game)->mana, &(game)->gemmes,&(game)->gemmes.lst_gemmes_file[*statut_gemme],&(game)->gemmes.lst_gemmes_file[x/ 80 ]);
                    *statut_gemme = -1;
                    return 0;
                }
            }
            else if(*statut_gemme > -1){
                if(est_case_grille(x,y) == 0 ){
                    add_gemme_tour(&(game->gemmes), &(game)->tours, &(game)->gemmes.lst_gemmes_file[*statut_gemme], y / LONGUEUR_CASE, x / LONGUEUR_CASE );
                    *statut_gemme = -1;
                }
            }
            else if (*statut_t == 1) {
                if (est_case_grille(x, y) != 1){
                    build_tour(game->plateau, &(game)->mana, &(game)->tours, y/LONGUEUR_CASE, x/LONGUEUR_CASE);
                }
            }
        }
    } else if (event == MLV_KEY) {
        if (key_sym == MLV_KEYBOARD_q) {
            return -1;
        } else if (key_sym == MLV_KEYBOARD_t) {
            *statut_t *= -1;
        }
    }
    return 0;
}

// a modifier
MLV_Color convert_angle_to_color(int angle) {
    // Convertissez l'angle HSL en RGB
    float h = angle / 360.0;
    float s = 1.0;  // Saturation à 100%
    float l = 0.5;  // Luminance à 50%

    // Convertissez de l'espace HSL à RGB
    float C = (1 - fabs(2 * l - 1)) * s;
    float X = C * (1 - fabs(fmod(h * 6, 2) - 1));
    float m = l - C / 2;

    float r, g, b;

    if (h < 1.0 / 6.0) {
        r = C;
        g = X;
        b = 0;
    } else if (h < 2.0 / 6.0) {
        r = X;
        g = C;
        b = 0;
    } else if (h < 3.0 / 6.0) {
        r = 0;
        g = C;
        b = X;
    } else if (h < 4.0 / 6.0) {
        r = 0;
        g = X;
        b = C;
    } else if (h < 5.0 / 6.0) {
        r = X;
        g = 0;
        b = C;
    } else {
        r = C;
        g = 0;
        b = X;
    }

    // Convertissez les composants de couleur de la plage [0, 1] à [0, 255]
    Uint8 red = (Uint8)((r + m) * 255);
    Uint8 green = (Uint8)((g + m) * 255);
    Uint8 blue = (Uint8)((b + m) * 255);

    // Utilisez votre fonction MLV_convert_rgba_to_color pour obtenir le MLV_Color
    return MLV_convert_rgba_to_color(red, green, blue, 255);
}


int est_case_grille(int x, int y){
    fprintf(stderr, "%d,%d\n", x, y);
    if (x > 28 * LONGUEUR_CASE || y > 22 * LONGUEUR_CASE){
        return 1;
    }
    return 0;
}


void affiche_tours(Game jeu, int statut_t ){
    int n = jeu.tours.nb_tours;
    for(int i = 0; i < n; i++){
        printf("%d\n", jeu.tours.lst_tours[i].gem.teinte );
        if(jeu.tours.lst_tours[i].gem.teinte != -1){
            MLV_draw_filled_circle(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 2, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 2, LONGUEUR_CASE / 2, convert_angle_to_color(jeu.tours.lst_tours[i].gem.teinte ));
            MLV_draw_text(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 3 - 10, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 3, "LVL %d", MLV_COLOR_BLACK, jeu.tours.lst_tours[i].gem.niveau);
        }
        else{
            MLV_draw_filled_circle(jeu.tours.lst_tours[i].position.y * LONGUEUR_CASE + LONGUEUR_CASE / 2, jeu.tours.lst_tours[i].position.x * LONGUEUR_CASE + LONGUEUR_CASE / 2, LONGUEUR_CASE / 2, MLV_COLOR_GREY);
        }
    }
    int  prix = 100 * (int)pow(2, jeu.tours.nb_tours - 3);
    if( statut_t == -1){
        MLV_draw_text_box(300,1200, 650, 20,"MAINTENEZ LA TOUCHE T POUR CONSTRUIRE UNE TOUR POUR %d DE MANA", 25, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, prix);
    }
    else{
        MLV_draw_text_box(300,1200, 650, 20,"EN CONSTRUCTION PRIX DE LA TOUR %d (REALCHEZ T POUR ARRETER DE CONSTRUIRE)", 25, MLV_COLOR_BLACK, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, prix);

    }
    
}


void affiche_monstres(Game jeu){
    int n = jeu.vagues.vague_actuelle + 1;
    for(int i = 0; i < n; i ++){
        int nb = jeu.vagues.tab[i].nb_monstres;
        for (int j = 0; j < nb; j++){
            MLV_draw_filled_circle(jeu.vagues.tab[i].monstres[j].y, jeu.vagues.tab[i].monstres[j].x, 15, MLV_COLOR_RED);
        }
    }
}


void info_vague(Game jeu, long int temps){
    MLV_draw_text_box(1100,1410,412,85, "LANCER LA VAGUE %d",25,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, jeu.vagues.vague_actuelle + 2);
    if(jeu.vagues.vague_actuelle >= 0){
        MLV_draw_text_box(0,1460, 500, 50, "VAGUE N°%d, TEMPS RESTANT AVANT LA PROCHAINE VAGUE %ld",25,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER, jeu.vagues.vague_actuelle + 1, temps);

    }
}