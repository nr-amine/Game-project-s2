#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Grille.h"
#include "Pion.h"
#include <unistd.h> // For usleep

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

    int n = 50, m = 50; // M dimensions
    Grille *M = Grille_initialiser(n, m);
    Grille_vider(M);

    Pion *pawn = allouer_Pion();
    pawn->x = 5;
    pawn->y = 5;
    pawn->old_x = 5;
    pawn->old_y = 5;
    M->Grille[pawn->y][pawn->x] = PION;

    srand(time(NULL));

    // Randomly place treasures
    int num_tresors = 5;
    for (int i = 0; i < num_tresors; i++) {
        int x, y;
        do {
            x = rand() % (n - 2) + 1; // Avoid placing on borders
            y = rand() % (m - 2) + 1;
        } while (M->Grille[y][x] != RIEN); // Ensure it's placed on an empty cell
        M->Grille[y][x] = TRESOR;
    }

    // Randomly place traps
    int num_pieges = 20;
    for (int i = 0; i < num_pieges; i++) {
        int x, y;
        do {
            x = rand() % (n - 2) + 1; // Avoid placing on borders
            y = rand() % (m - 2) + 1;
        } while (M->Grille[y][x] != RIEN); // Ensure it's placed on an empty cell
        M->Grille[y][x] = PIEGE;
    }

    clear();
    Grille_redessiner(M);
    refresh();

    // Game loop
    int ch;
    while (1) {
        // Handle user input
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
            case 'q': // Quit the game
                goto end_game;
        }

        // Move the pawn if there is user input
        if (move != -1) {
            int game_over = Pion_deplacer(move, pawn, M);
            if (game_over) {
                break; // Exit the game loop if the game is over
            }
        }

        // Move traps periodically
        static int trap_timer = 0;
        trap_timer++;
        if (trap_timer >= 10) { // Adjust this value to control trap movement speed
            Bouger_pieges(M);
            trap_timer = 0;
        }

        // Redraw the M
        clear();
        Grille_redessiner(M);
        printw("position de pion: (%d, %d)\n", pawn->x, pawn->y);
        printw("Utiliser les fleches pour bouger, 'q' pour quitter\n");
        refresh();

        // Delay to control the game speed
        usleep(50000); // 50ms delay
    }

end_game:
    pion_desallouer(pawn);
    Grille_desallouer(M);
    endwin();
    return 0;
}