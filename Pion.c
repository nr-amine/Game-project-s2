#include "Grille.h"
#include <stdio.h>
#include <stdlib.h>
#include "Pion.h"
#include <ncurses.h>

Pion* allouer_Pion() {
    Pion* Pawn = malloc(sizeof(Pion));
    if (Pawn == NULL) {
        exit(2);
    }
    return Pawn;
}

void pion_desallouer(Pion *P) {
    free(P);
}

int placer_pion(int x, int y, Pion *P, Grille *M) {
    if (x < 0 || x >= M->n || y < 0 || y >= M->m) {
    return 0;
    }
    if (M->Grille[y][x] == TRESOR) {
        P->old_x = P->x;
        P->old_y = P->y;
        P->x = x;
        P->y = y;
        M->Grille[P->y][P->x] = PION;
        return 2;
    }
    if (M->Grille[y][x] == PIEGE) {
        P->old_x = P->x;
        P->old_y = P->y;
        P->x = x;
        P->y = y;
        M->Grille[P->y][P->x] = PION;
        return -1;
    }
    if (M->Grille[y][x] != RIEN) {
        return 0;
    }
    P->old_x = P->x;
    P->old_y = P->y;
    P->x = x;
    P->y = y;
    return 1;
}

int Pion_deplacer(Move E, Pion *P, Grille *M) {
    int result;
    switch (E) {
        case HAUT:
            result = placer_pion(P->x, P->y - 1, P, M);
            break;
        case BAS:
            result = placer_pion(P->x, P->y + 1, P, M);
            break;
        case DROITE:
            result = placer_pion(P->x + 1, P->y, P, M);
            break;
        case GAUCHE:
            result = placer_pion(P->x - 1, P->y, P, M);
            break;
        default:
            return 0; // Mouvement invalide
    }

    switch (result) {
        case 1: // Déplacement valide
            M->Grille[P->old_y][P->old_x] = RIEN; // Efface l'ancienne position
            M->Grille[P->y][P->x] = PION;        // Place le pion à la nouvelle position
            return 0; // Le jeu continue
        case 2: // Trésor collecté
            M->Grille[P->old_y][P->old_x] = RIEN;
            M->Grille[P->y][P->x] = PION;
            return 0; // Le jeu continue
        case -1: // Le pion touche un piège
            clear(); // Efface l'écran
            int centre_ligne = M->m / 2;
            int centre_colonne = (M->n - 9) / 2; // "Game Over" fait 9 caractères
            for (int i = 0; i < centre_ligne; i++) {
                printw("\n");
            }
            for (int i = 0; i < centre_colonne; i++) {
                printw(" ");
            }
            printw("Game Over\n");
            refresh(); // Met à jour l'écran pour afficher "Game Over"

            // Désactive l'entrée 
            nodelay(stdscr, FALSE); 
            // Solution pour l'affichage de "Game Over", Je ne sais pas si c'est la meilleure, solution trouvée sur stackoverflow
            getch(); // Attend que l'utilisateur appuie sur une touche
            nodelay(stdscr, TRUE); // Réactive l'entrée 
            return 1; // Le jeu se termine
        case 0: // Mouvement invalide (mur ou bordure)
            return 0; // Le jeu continue
    }

    return 0; // Cas par défaut
}

void Bouger_pieges(Grille *M) {
    for (int i = 0; i < M->m; i++) {
        for (int j = 0; j < M->n; j++) {
            if (M->Grille[i][j] == PIEGE) {
                int direction = rand() % 4;
                int new_x = j, new_y = i;

                switch (direction) {
                    case 0: new_y--; break; // Haut
                    case 1: new_y++; break; // Bas
                    case 2: new_x--; break; // Gauche
                    case 3: new_x++; break; // Droite
                }

                if (new_x >= 0 && new_x < M->n && new_y >= 0 && new_y < M->m && M->Grille[new_y][new_x] == RIEN) {
                    M->Grille[i][j] = RIEN;
                    M->Grille[new_y][new_x] = PIEGE;
                }
            }
        }
    }
}