#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Grille.h"
#include "Pion.h"
#include <unistd.h> // For usleep

#define COLOR_GRAY 16
#define COLOR_ORANGE 17


int main() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);   // RIEN
    init_pair(2, COLOR_BLACK, COLOR_WHITE);   // BORDURE
    init_pair(3, COLOR_WHITE, COLOR_RED);     // MUR
    init_pair(4, COLOR_WHITE, COLOR_CYAN);    // PION
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);  // PIEGE
    init_pair(6, COLOR_BLACK, COLOR_GREEN);   // TRESOR
    init_pair(7, COLOR_WHITE, COLOR_MAGENTA);  // Sortie
    init_color(COLOR_GRAY, 500, 500, 500);
    init_pair(8, COLOR_BLACK, COLOR_GRAY);    // CROCHETAGE
    init_color(COLOR_ORANGE, 1000, 500, 0);
    init_pair(9, COLOR_BLACK, COLOR_ORANGE);  // DYNAMITE

    int n = 80, m = 45; 
    Grille *M = Grille_initialiser(n, m);
    Grille_vider(M);

    Pion *pawn = allouer_Pion();
    pawn->x = 5;
    pawn->y = 5;
    pawn->old_x = 5;
    pawn->old_y = 5;
    M->Grille[pawn->y][pawn->x] = PION;

    srand(time(NULL));

    Populer_labyrinthe(M, n, m, pawn, true);

    clear();
    Grille_redessiner(M);
    refresh();

    int ch;
    int comp_piege = 0;
    while (1) {
        ch = getch();
        Move move = -1;
        switch (ch) {
            case KEY_UP:
                move = HAUT;
                break;
            case KEY_DOWN:
                move = BAS;
                break;
            case KEY_LEFT:
                move = GAUCHE;
                break;
            case KEY_RIGHT:
                move = DROITE;
                break;
            case 'e':
                Crocheter(pawn, M);
                break;
            case 'd':
                utiliser_dynamite(pawn, M);
                break;
            case 'q': // Quitter le jeu
                pion_desallouer(pawn);
                Grille_desallouer(M);
                endwin(); 
                return 0;
        }

        if (move != -1) {
            int game_over = Pion_deplacer(move, pawn, M);
            if (game_over) {
                break; 
            }
        }

        // Mouvement des pièges
        comp_piege++;
        if (comp_piege >= 20) { // Modifier cette valeur pour ajuster la vitesse des pièges
            Bouger_pieges(M);
            comp_piege = 0;
        }

        Populer_labyrinthe(M, n, m, pawn, false);

        clear();
        Grille_redessiner(M);
        printw("position de pion: (%d, %d)\n", pawn->x, pawn->y);
        printw("Utiliser les fleches pour bouger, 'q' pour quitter\n");
        refresh();

        
        usleep(50000); // 50ms 
    }
}