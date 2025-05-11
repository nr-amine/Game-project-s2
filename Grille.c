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
                M->Grille[i][j] = MUR; 
            }
        }
    }

    int start_x = 1 + rand() % (M->n - 2);
    int start_y = 1 + rand() % (M->m - 2);
    M->Grille[start_y][start_x] = RIEN; 
    CarveMaze(M, start_x, start_y, 3); 
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
                case CROCHETER:
                    attron(COLOR_PAIR(8));
                    printw("  ");
                    attroff(COLOR_PAIR(8));
                    break;
                case DYNAMITE:
                    attron(COLOR_PAIR(9));
                    printw("  ");
                    attroff(COLOR_PAIR(9));
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

/*Algorithme de labyrinthe adapté de https://raw.githubusercontent.com/joewing/maze/master/maze.c, 
  modifié car le code original est fait pour une matrice 1D*/

void CarveMaze(Grille *M, int x, int y, int epaisseur) {
    int x1, y1;
    int x2, y2;
    int dx, dy;
    int dir, count;

    dir = rand() % 4;
    count = 0;
    while (count < 4) {
        dx = 0; dy = 0;
        switch (dir) {
            case 0:  dx = 1;  break; 
            case 1:  dy = 1;  break; 
            case 2:  dx = -1; break; 
            default: dy = -1; break; 
        }
        x1 = x + dx * epaisseur;
        y1 = y + dy * epaisseur;
        x2 = x1 + dx * epaisseur;
        y2 = y1 + dy * epaisseur;

        // Verifier les bordures et si c'est possible de placer les murs
        if (x2 > 0 && x2 < M->n && y2 > 0 && y2 < M->m) {
            bool peut_sculpter = true;
            for (int i = 0; i < epaisseur; i++) {
                for (int j = 0; j < epaisseur; j++) {
                    if (M->Grille[y1 + i][x1 + j] != MUR || M->Grille[y2 + i][x2 + j] != MUR) {
                        peut_sculpter = false;
                        break;
                    }
                }
                if (!peut_sculpter) break;
            }

            if (peut_sculpter) {
                // sculpter le labyrinthe
                for (int i = 0; i < epaisseur; i++) {
                    for (int j = 0; j < epaisseur; j++) {
                        M->Grille[y1 + i][x1 + j] = RIEN;
                        M->Grille[y2 + i][x2 + j] = RIEN;
                    }
                }
                x = x2; y = y2; // Avancer
                dir = rand() % 4; // chosir une direction aléatoire
                count = 0;

                if (rand() % 3 == 0) {
                    CarveMaze(M, x, y, epaisseur);
                }
            } else {
                dir = (dir + 1) % 4; 
                count += 1;
            }
        } else {
            dir = (dir + 1) % 4; 
            count += 1;
        }
    }
}


void Populer_labyrinthe(Grille *M, int n, int m, Pion *P, bool status) {
    int num_tresors = 5;
    int num_pieges = 20;
    int num_crochetage = 7;
    int num_dynamite = 5;

    if (status) {
        for (int i = 0; i < num_tresors; i++) {
            int x, y;
            do {
                x = rand() % (n - 2) + 1; 
                y = rand() % (m - 2) + 1;
            } while (M->Grille[y][x] != RIEN); 
            M->Grille[y][x] = TRESOR;
        }

        for (int i = 0; i < num_pieges; i++) {
            int x, y;
            do {
                x = rand() % (n - 2) + 1;
                y = rand() % (m - 2) + 1;
            } while (M->Grille[y][x] != RIEN);
            M->Grille[y][x] = PIEGE;
        }

        for (int i = 0; i < num_crochetage; i++) {
            int x, y;
            do {
                x = rand() % (n - 2) + 1;
                y = rand() % (m - 2) + 1;
            } while (M->Grille[y][x] != RIEN);
            M->Grille[y][x] = CROCHETER;
        }

        for (int i = 0; i < num_dynamite; i++) {
            int x, y;
            do {
                x = rand() % (n - 2) + 1;
                y = rand() % (m - 2) + 1;
            } while (M->Grille[y][x] != RIEN);
            M->Grille[y][x] = DYNAMITE;
        }
        
    } else {
        if (P->score == num_tresors) {
            int num_sorties = 1;
            for (int i = 0; i < num_sorties; i++) {
                int x, y;
                do {
                    x = rand() % (n - 2) + 1; 
                    y = rand() % (m - 2) + 1;
                } while (M->Grille[y][x] != RIEN); 
                M->Grille[y][x] = SORTIE;
            }
            P->score = 0;
        }
    }
}