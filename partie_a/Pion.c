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

        case SORTIE:
        bg_pion(P);
        M->Grille[P->y][P->x] = PION;
            return 3;

        case RIEN:
        bg_pion(P);
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
            case 0: // Mouvement invalide (mur ou bordure)
            return 0; 
    }

    return 0; // Cas par défaut
}


//Partie pour undo et liste chainée
void ajouter_bouge(Bouge **tete, Move E) {
    Bouge *nouveau = malloc(sizeof(Bouge));
    if (nouveau == NULL) {
        exit(2);
    }
    nouveau->E = E;
    nouveau->next = *tete;
    *tete = nouveau;
}

void supprimer_bouge(Bouge **tete) {
    if (*tete == NULL) {
        return;
    }
    Bouge *temp = *tete;
    *tete = (*tete)->next;
    free(temp);
}

Move inverse_move(Move E) {
    switch (E) {
        case HAUT:
            E = BAS;
            break;
        case BAS:
            E = HAUT;
            break;
        case GAUCHE:
            E = DROITE;
            break;
        case DROITE:
            E = GAUCHE;
            break;
    }
    return E;
}

void Undo(Bouge **tete, Pion *P, Grille *M, Move *dern_move) {
    if (*tete == NULL) {
        *(dern_move) = -1;
        return;
    }
    Bouge *dernier = *tete;
    Move E = dernier->E;
    E = inverse_move(E);
    Pion_deplacer(E, P, M);
    supprimer_bouge(tete);
}
