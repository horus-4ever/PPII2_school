CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wno-unused-parameter
CDEBUGFLAGS = -g -fsanitize=address
RELEASEFLAGS = -O3
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

EXEC1 = main
SRC_STRINGS = $(shell find utf8strings -name '*.c')
SRC_DATASRTUCTURES = $(shell find datastructures -name '*.c')
SRC1 = $(shell find animations -name '*.c') $(shell find quest -name '*.c') $(shell find tnobject -name '*.c') $(shell find questparser -name '*.c') $(shell find utf8strings -name '*.c') $(shell find datastructures -name '*.c') $(shell find widgets -name '*.c') game.c parameters.c interpreter.c fonctions_SDL.c perso_SDL.c main.c map.c map_SDL.c npc.c menu.c sound.c items.c
OBJ1=$(SRC1:.c=.o)

EXECTEST = testing
SRCTEST = tests.c map.c map_SDL.c fonctions_SDL.c perso_SDL.c datastructures/array.c $(SRC_DATASRTUCTURES) $(SRC_STRINGS)
OBJTEST = $(SRCTEST:.c=.o)

all: $(EXEC1)
	./$<

debug: CFLAGS += $(CDEBUGFLAGS)
debug: $(EXEC1)

release: CFLAGS += $(RELEASEFLAGS)
release: $(EXEC1)

$(EXEC1):$(OBJ1)
	@ $(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test:$(EXECTEST)
	./$<

$(EXECTEST):$(OBJTEST)
	@ $(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

*.o:*.c
	@ $(CC) $(CFLAGS) -c $< -o $@

run:
	@ ./$(EXEC1)

clean:
	@ rm -f $(OBJ1) $(EXEC1) $(OBJTEST) $(EXECTEST)