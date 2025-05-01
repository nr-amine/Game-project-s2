#ifndef _FONCTIONS_PION_
#define _FONCTIONS_PION_

typedef enum {HAUT, BAS, DROITE, GAUCHE} Move;

typedef struct {
    int x,y;
    int old_x, old_y;
    int score;
} Pion;


Pion* allouer_Pion();
void pion_desallouer();
int placer_pion();
int Pion_deplacer();
void Bouger_pieges();
#endif
