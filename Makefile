all: jeu

Grille.o: Grille.c Grille.h 
	gcc -c -Wall Grille.c

Pion.o: Pion.c Pion.h
	gcc -c -Wall Pion.c

main.o: main.c Grille.h Pion.h
	gcc -c -Wall main.c

jeu: main.o Grille.o Pion.o 
	gcc -o jeu main.o Grille.o Pion.o -lncurses

clean:
	rm -fr *.o jeu
