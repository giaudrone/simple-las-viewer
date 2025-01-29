CC = gcc
CFLAGS = -Wall -g

ifeq ($(OS),Windows_NT)
    LDFLAGS = -lopengl32 -lglfw3 -lgdi32 -static-libgcc -static-libstdc++
else
    LDFLAGS = -lglfw -lGL -lm
endif

SRC = src/main.c src/prints.c src/renderWindow.c includes/glad.c
EXE = simplelasviewer

all:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXE)

