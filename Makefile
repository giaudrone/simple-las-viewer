CC = gcc
CFLAGS = -Wall -g

	LDFLAGS = -lglfw -lGL -lm -pg

SRC = src/main.c src/renderWindow.c includes/glad.c
EXE = simple-las-viewer

all:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXE)
