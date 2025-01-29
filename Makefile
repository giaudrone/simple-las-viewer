CC = gcc
CFLAGS = -Wall -g

	LDFLAGS = -lglfw -lGL -lm

SRC = src/main.c src/prints.c src/renderWindow.c includes/glad.c
EXE = simplelasviewer

all:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXE)

