# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Detect OS
ifeq ($(OS),Windows_NT)
    PLATFORM = WINDOWS
    LDFLAGS = -lopengl32 -lglfw3 -lgdi32 -static-libgcc -static-libstdc++
else
    PLATFORM = LINUX
    LDFLAGS = -lglfw -lGL -lm
endif

# Source files
SRC = main.c prints.c renderWindow.c includes/glad.c
OBJ = $(SRC:.c=.o)
EXE = simplelasviewer

# Build rules
all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJ) $(EXE)

