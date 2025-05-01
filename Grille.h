#ifndef _FONCTIONS_JEU_
#define _FONCTIONS_JEU_

typedef enum {RIEN, MUR, PION, PIEGE, BORDURE, TRESOR} Event;

typedef struct {
    int n, m;
    Event** Grille;
} Grille;

Grille *Grille_initialiser(int n, int m);
void Grille_desallouer(Grille *grille);
void Grille_vider(Grille *grille);
void Grille_redessiner(Grille *M);
void CarveMaze(Grille *M, int x, int y, int epeceur);

#endif
