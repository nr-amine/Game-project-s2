#include <stdio.h>
#include <stdlib.h>
#include "Grille.h"
#include "Pion.h"
#include <ncurses.h>




Grille *Grille_initialiser(int n, int m) {
    Grille *grille_line = malloc(sizeof(Grille));
    grille_line->Grille = malloc(sizeof(Event*)*m);
    for(int i=0; i<m; i++) {
        grille_line->Grille[i] = malloc(sizeof(Event)*n);
    }
    grille_line->n = n;
    grille_line->m = m;
    return grille_line;
}

void Grille_desallouer(Grille *M) {
    for(int i=0; i<M->m; i++) {
        free(M->Grille[i]);
    }
    free(M->Grille);
    free(M);
}

void Grille_vider(Grille *M) {
    for (int i = 0; i < M->m; i++) {
        for (int j = 0; j < M->n; j++) {
            if (i == 0 || i == M->m - 1 || j == 0 || j == M->n - 1) {
                M->Grille[i][j] = BORDURE; 
            } else {
                M->Grille[i][j] = RIEN; 
            }
        }
    }
}


/*Utilisation de COLOR_PAIR et printw a cause de problémes d'affichage avec printf, 
  donc j'ai choisi ncurses comme solution d'entrée ET sortie*/

void Grille_redessiner(Grille *M) {
    for(int i=0; i<M->m; i++) {
        for (int j=0; j<M->n; j++) {
            switch (M->Grille[i][j]) {
                case RIEN:
                    attron(COLOR_PAIR(1));
                    printw("  ");
                    attroff(COLOR_PAIR(1));
                    break;
                case BORDURE:
                    attron(COLOR_PAIR(2));
                    printw("  ");
                    attroff(COLOR_PAIR(2));
                    break;
                case MUR:
                    attron(COLOR_PAIR(3));
                    printw("  ");
                    attroff(COLOR_PAIR(3));
                    break;
                case PION:
                    attron(COLOR_PAIR(4));
                    printw("  ");
                    attroff(COLOR_PAIR(4));
                    break;
                case PIEGE:
                    attron(COLOR_PAIR(5));
                    printw("  ");
                    attroff(COLOR_PAIR(5));
                    break;
                case TRESOR:
                    attron(COLOR_PAIR(6));
                    printw("  ");
                    attroff(COLOR_PAIR(6));
                    break;
                case SORTIE:
                    attron(COLOR_PAIR(7));
                    printw("  ");
                    attroff(COLOR_PAIR(7));
                    break;
                default:
                    attron(COLOR_PAIR(1));
                    printw("  ");
                    attroff(COLOR_PAIR(1));
                    break;               
            }
        }
        printw("\n");
    }
}
