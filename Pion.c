#include "Grille.h"
#include <stdio.h>
#include <stdlib.h>
#include "Pion.h"
#include <ncurses.h>
#include <stdbool.h>

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

int placer_pion(int x, int y, Pion *P, Grille *M, int *score) {
    void bg_pion(Pion *P) {
            P->old_x = P->x;
            P->old_y = P->y;
            P->x = x;
            P->y = y;
    }
    if (x < 0 || x >= M->n || y < 0 || y >= M->m) {
        return 0;
    }

    switch (M->Grille[y][x]) {
        case TRESOR:
        bg_pion(P);
        M->Grille[P->y][P->x] = PION;
        (*score)++;
        return 1;

        case PIEGE:
        bg_pion(P);
        M->Grille[P->y][P->x] = PION;
            return -1;

        case SORTIE:
        bg_pion(P);
        M->Grille[P->y][P->x] = PION;
            return 3;

        case RIEN:
        bg_pion(P);
        return 1;

        case CROCHETER:
        bg_pion(P);
        P->Inventaire.Crocheter++;
        return 1;

        case DYNAMITE:
        bg_pion(P);
        P->Inventaire.Dynamite++;
        return 1;

        default:
            return 0;
    }
}

int Pion_deplacer(Move E, Pion *P, Grille *M) {
    int result;
    switch (E) {
        case HAUT:
            result = placer_pion(P->x, P->y - 1, P, M, &P->score);
            break;
        case BAS:
            result = placer_pion(P->x, P->y + 1, P, M, &P->score);
            break;
        case DROITE:
            result = placer_pion(P->x + 1, P->y, P, M, &P->score);
            break;
        case GAUCHE:
            result = placer_pion(P->x - 1, P->y, P, M, &P->score);
            break;
        default:
            return 0; // Mouvement invalide
    }

    switch (result) {
        case 1: // Déplacement valide
            M->Grille[P->old_y][P->old_x] = RIEN; // Efface l'ancienne position
            M->Grille[P->y][P->x] = PION;        // Place le pion à la nouvelle position
            return 0; 
        case -1: // Le pion touche un piège
            clear(); 
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
            return 0; 
            case 3: // Sortie atteinte
            // Efface l'ancienne position du pion
            M->Grille[P->old_y][P->old_x] = RIEN;
            Grille_vider(M);
            // Trouve une nouvelle position valide pour le pion
            int x, y;
            do {
                x = rand() % (M->n - 2) + 1;
                y = rand() % (M->m - 2) + 1;
            } while (M->Grille[y][x] != RIEN); // Trouver une cellule vide

            // Met à jour la position du pion
            P->x = x;
            P->y = y;

            // Place le pion à la nouvelle position
            M->Grille[P->y][P->x] = PION;

            // Réinitialise la grille pour un nouveau labyrinthe
            Populer_labyrinthe(M, M->n, M->m, P, true);
            return 0;
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

void Crocheter(Pion *P, Grille *M) {
    if (P->Inventaire.Crocheter > 0) {
        for (int i = P->y - 1; i <= P->y + 1; i++) {
            for (int j = P->x - 1; j <= P->x + 1; j++) {
                if (i >= 0 && i < M->m && j >= 0 && j < M->n) {
                    if (M->Grille[i][j] == PIEGE) {
                        P->Inventaire.Crocheter--;
                        M->Grille[i][j] = RIEN;
                    }
                }
            }
        }
    }
}

void utiliser_dynamite(Pion *P, Grille *M) {
    if (P->Inventaire.Dynamite <= 0) return;

    // Vérifier directement les 4 côtés
    if (M->Grille[P->y-1][P->x] == MUR) {        // Haut
        for (int i = 1; i <= 3; i++)
            if (P->y-i >= 0) M->Grille[P->y-i][P->x] = RIEN;
    }
    else if (M->Grille[P->y+1][P->x] == MUR) {   // Bas
        for (int i = 1; i <= 3; i++)
            if (P->y+i < M->m) M->Grille[P->y+i][P->x] = RIEN;
    }
    else if (M->Grille[P->y][P->x-1] == MUR) {   // Gauche
        for (int i = 1; i <= 3; i++)
            if (P->x-i >= 0) M->Grille[P->y][P->x-i] = RIEN;
    }
    else if (M->Grille[P->y][P->x+1] == MUR) {   // Droite
        for (int i = 1; i <= 3; i++)
            if (P->x+i < M->n) M->Grille[P->y][P->x+i] = RIEN;
    }

    P->Inventaire.Dynamite--;
}