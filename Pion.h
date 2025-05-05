#ifndef _FONCTIONS_PION_
#define _FONCTIONS_PION_

typedef enum {HAUT, BAS, DROITE, GAUCHE} Move;


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
#endif
