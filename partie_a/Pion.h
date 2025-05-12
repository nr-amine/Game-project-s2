#ifndef _FONCTIONS_PION_
#define _FONCTIONS_PION_

typedef enum {HAUT, BAS, DROITE, GAUCHE, UNDO} Move;

typedef struct Bouge {
    Move E;
    struct Bouge *next;
} Bouge;


struct Inventaire
{
    int Crocheter;
    int Dynamite;
};


typedef struct {
    int x,y;
    int old_x, old_y;
    struct Inventaire Inventaire;
    int score;
} Pion;


Pion* allouer_Pion();
void pion_desallouer();
int placer_pion();
int Pion_deplacer();
void Bouger_pieges();
void Crocheter();
void utiliser_dynamite();
void Undo();
void ajouter_bouge();

#endif
