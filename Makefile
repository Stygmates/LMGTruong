SRCDIR = src/
HEADERDIR = include/
DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS = -O2 -Wall -DDEBUG
else
    CFLAGS= -O2 -Wall
endif

LD ?= 1
ifeq ($(LD), 1)

	LOADER = MeshLoader.o
else
	LOADER = Loader.o
	LFLAG = -DLOADER
endif

NORM ?= 1
ifeq ($(NORM), 1)
	NFLAGS = -DNORM
endif

LINES ?= 0
ifeq ($(LINES), 1)
	LFLAG2 = -DLINES
endif
CC = g++
LDFLAGS = -lGL -lglut -lGLEW -Iglm -lSOIL -lassimp -Iassimp -I /usr/include/SOIL
EXEC = main
OBJ = $(SRC:.c=.o)

all: clean main

main.o: $(OBJ) $(LOADER) My3DModel.o Texture.o Shader.o
	$(CC) $(CFLAGS) $(NFLAGS) $(LFLAG2) -std=c++11 -o main.o exo1.cpp $^ $(LDFLAGS)

main: $(LOADER) My3DModel.o Texture.o Shader.o
	$(CC) $(CFLAGS) -std=c++11 -o main $(SRCDIR)main.cpp $^ $(LDFLAGS)

Loader.o: $(SRCDIR)Loader.cpp $(HEADERDIR)Loader.hpp
	$(CC) $(CFLAGS) -g -c $<

My3DModel.o: $(SRCDIR)My3DModel.cpp $(HEADERDIR)My3DModel.hpp
	$(CC) $(CFLAGS) $(LFLAG) $(NFLAGS) -g -c $<

Texture.o: $(SRCDIR)Texture.cpp $(HEADERDIR)Texture.hpp
	$(CC) $(CFLAGS) -g -c $< $(LDFLAGS)

MeshLoader.o: $(SRCDIR)MeshLoader.cpp $(HEADERDIR)MeshLoader.hpp
	$(CC) $(CFLAGS) -std=c++11 -g -c $<

%.o: $(SRCDIR)%.cpp $(HEADERDIR)%.hpp
	$(CC) $(CFLAGS) -std=c++11 -g -c $< $(LDFLAGS)

.PHONY: clean lib-deps

lib-deps:
	chmod u+x install.sh
	./install.sh
clean:
	/bin/rm -f $(EXEC) *.o debug/*
