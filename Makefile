CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = jeu

SRCS = main.c Grille.c Pion.c

OBJS = $(SRCS:.c=.o)

HEADERS = Grille.h Pion.h

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lncurses

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean $(TARGET)
