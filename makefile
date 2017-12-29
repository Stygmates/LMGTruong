DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS = -O2 -Wall -DDEBUG
else
    CFLAGS= -O2 -Wall
endif

LD ?= 1
ifeq ($(LD), 1)
	LOADER = Loader.o
	LFLAG = -DLOADER
else
	LOADER = MeshLoader.o
endif

NORM ?= 1
ifeq ($(NORM), 1)
	NFLAGS = -DNORM
endif

CC = g++
LDFLAGS = -lGL -lglut -lGLEW -Iglm -lSOIL -lassimp -Iassimp -I /usr/include/SOIL
EXEC = main
OBJ = $(SRC:.c=.o)

all: clean main

main: $(OBJ) $(LOADER) My3DModel.o Texture.o
	$(CC) $(CFLAGS) $(NFLAGS) -std=c++11 -o $(EXEC) exo1.cpp $^ $(LDFLAGS)

Loader.o: Loader.cpp Loader.hpp
	$(CC) $(CFLAGS) -g -c $<

My3DModel.o: My3DModel.cpp My3DModel.hpp
	$(CC) $(CFLAGS) $(LFLAG) $(NFLAGS) -g -c $<

Texture.o: Texture.cpp Texture.hpp
	$(CC) $(CFLAGS) -g -c $< $(LDFLAGS)

MeshLoader.o: MeshLoader.cpp MeshLoader.hpp
	$(CC) $(CFLAGS) -std=c++11 -g -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -g -c $< 

.PHONY: clean
clean:
	/bin/rm -f $(EXEC) *.o debug/*