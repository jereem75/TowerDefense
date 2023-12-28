#include <stdio.h>
#include <stdlib.h>
#include "../include/graphique.h"
#include "../include/generation_terrain.h"
#include "../include/gestion_gemme.h"
#include <MLV/MLV_all.h>

void afficher_plateau(Terrain *jeu) {
    int i, j;
    for (i = 0; i < LIG; i++) {
        for (j = 0; j < COL; j++) {
            MLV_draw_filled_rectangle(j* 53, i * 53, 53 , 53, MLV_COLOR_GREEN4);
            MLV_draw_rectangle(j * 53, i * 53, 53, 53, MLV_COLOR_BLACK);
        }
    }
    for(int i = 0; i < jeu->longueur; i++){
        MLV_draw_filled_rectangle(jeu->chemin[i].y * 53, jeu->chemin[i].x * 53, 53 , 53, MLV_COLOR_YELLOW1);
        MLV_draw_rectangle(jeu->chemin[i].y * 53, jeu->chemin[i].x * 53, 53 , 53, MLV_COLOR_BLACK);
    }
    MLV_draw_filled_rectangle(jeu->chemin[0].y * 53, jeu->chemin[0].x * 53, 53 , 53, MLV_COLOR_PURPLE1);
    MLV_draw_filled_rectangle(jeu->chemin[jeu->longueur - 1].y * 53, jeu->chemin[jeu->longueur - 1].x * 53, 53 , 53, MLV_COLOR_RED);
    
}


void affiche_file_gemme(Game jeu){
    int length = jeu.gemmes.nb_gemmes_file;
    for(int i = 0; i < 10; i++){
        MLV_draw_filled_rectangle(i * 50, 1300, 80, 80, MLV_COLOR_WHITE);
        MLV_draw_rectangle(i * 50, 1300, 80, 80, MLV_COLOR_BLACK); 
        if (i < length) {
            unsigned int N = jeu.gemmes.lst_gemmes_file[i].teinte;
            unsigned char A = N % 256;
            N /= 256;
            unsigned char B = N % 256;
            N /= 256;
            unsigned char G = N % 256;
            N /= 256;
            unsigned char R = N % 256;
            MLV_draw_filled_circle(i * 50 + 25, 1340, 20, MLV_convert_rgba_to_color(B, G, R, A));
        }
    }
}





void affiche_infos(){
    MLV_draw_adapted_text_box(0,1236,"Gemmes générées:", 25, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER);
    MLV_draw_text_box(1200,1200,150,100,"Generer une gemme",25,MLV_COLOR_BLACK,MLV_COLOR_BLACK,MLV_COLOR_WHITE,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
}

void affiche_mana(Mana mana){
    MLV_draw_adapted_text_box(0,1170,"Mana: %d", 25, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_TEXT_CENTER, mana.nb_mana);
}

int clic(Game *game) {
    MLV_Event event;
    MLV_Keyboard_button key_sym;
    MLV_Mouse_button mouse_button;
    int unicode;
    char *texte;
    MLV_Input_box *input_box;
    int x, y;
    MLV_Button_state state;

    event = MLV_wait_event(&key_sym, NULL, &unicode, &texte, &input_box, &x, &y, &mouse_button, &state);

    if (event == MLV_MOUSE_BUTTON) {
        if (state == MLV_PRESSED) {
            if (x > 1200 && x < 1350 && y > 1200 && y < 1300) {
                cree_gemme(&(game)->mana, &(game)->gemmes, 0);
                return 0;
            }
        } else if (state == MLV_RELEASED) {
            // Code à exécuter lorsque le bouton de la souris est relâché
        }
    } else if (event == MLV_KEY) {
        if (key_sym == MLV_KEYBOARD_q) {
            return -1;
        }
    }

    return 0;
}