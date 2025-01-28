# Compiler and flags
CC = gcc
CFLAGS = -I src/include -Wall -Wextra -g
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm

# Executable name
TARGET = SDL2_Project

# Source files
SRC = \
    menu.c \
    game.c \
    main.c \
    dominos_events.c \
    dominos_game.c \
    dominos_renderer.c \
    dominos_types.c \
    triomino_events.c \
    triomino_game.c \
    triomino_renderer.c \
    triomino_types.c \
    player_setup.c

# Object files
OBJ = $(SRC:.c=.o)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)
