#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Grille.h"
#include "Pion.h"
#include <unistd.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Minimum de 2 arguments requis\n");
        return 1;
    }
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

    int n = 80, m = 45; // Dimensions
    Grille *M = Grille_initialiser(n, m);
    Grille_vider(M);

    Pion *pion = allouer_Pion();
    pion->x = 5;
    pion->y = 5;
    pion->old_x = 5;
    pion->old_y = 5;
    M->Grille[pion->y][pion->x] = PION;

    clear();
    Grille_redessiner(M);
    refresh();

    int ch;
int comp_piege = 0;
Move dern_move = -1;
bool peut_undo = false;
if (argv[1][0]=='1') {
while (1) {
    ch = getch();
    Move move = -1;

    switch (ch) {
        case KEY_UP:
            move = HAUT;
            dern_move = HAUT;
            break;
        case KEY_DOWN:
            move = BAS;
            dern_move = BAS;
            break;
        case KEY_LEFT:
            move = GAUCHE;
            dern_move = GAUCHE;
            break;
        case KEY_RIGHT:
            move = DROITE;
            dern_move = DROITE;
            break;
        case 'q': // Quitter le jeu
            goto end_game;
    }


    if (move == -1 && dern_move != -1) {
        move = dern_move;
    }


    if (move != -1) {
        int game_over = Pion_deplacer(move, pion, M);
        if (game_over) {
            break;
        }
    }

    clear();
    Grille_redessiner(M);
    printw("position de pion: (%d, %d)\t\t'u' pour faire un undo\n", pion->x, pion->y);
    printw("Utiliser les fleches pour bouger, 'q' pour quitter\n");
    refresh();

    usleep(50000); // 50ms
}
}
else if (argv[1][0]=='2') {
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
            case 'u':
                if (!peut_undo) {
                    move = -1;
                }
                else {
                    move = UNDO;
                }
                break;
            case 'q':
                goto end_game;
        }

        if (move == UNDO) {
            Undo(&dern_move, pion, M, &dern_move);
            if (dern_move == -1) {
                printw("Impossible de faire un undo\n");
                peut_undo = false;
                refresh();
            }
            continue;
        }

        if (move != -1) {
            int game_over = Pion_deplacer(move, pion, M);
            ajouter_bouge(&dern_move, move);
            peut_undo = true;
            if (game_over) {
                break;
            }
        }

        clear();
        Grille_redessiner(M);
        printw("position de pion: (%d, %d)\n", pion->x, pion->y);
        printw("Utiliser les fleches pour bouger, 'q' pour quitter\n");
        refresh();

        usleep(50000); // 50ms
    }
}
else if (argv[1][0]!='1' && argv[1][0]!='2') {
    printf("Argument invalide. Utilisez '1' ou '2'.\n");
    goto end_game;
}


end_game:
    Grille_desallouer(M);
    pion_desallouer(pion);
    endwin();
    return 0;
}