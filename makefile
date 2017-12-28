DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS = -O2 -Wall -DDEBUG
else
    CFLAGS= -O2 -Wall
endif

CC = g++
LDFLAGS = -lGL -lglut -lGLEW -Iglm -lSOIL -lassimp -Iassimp -I /usr/include/SOIL
EXEC = main
OBJ = $(SRC:.c=.o)

all: $(OBJ) MeshLoader.o My3DModel.o Texture.o
	$(CC) $(CFLAGS) -std=c++11 -o $(EXEC) exo1.cpp $^ $(LDFLAGS)

Loader.o: Loader.cpp Loader.hpp
	$(CC) $(CFLAGS) -g -c $<

My3DModel.o: My3DModel.cpp My3DModel.hpp
	$(CC) $(CFLAGS) -g -c $<

Texture.o: Texture.cpp Texture.hpp
	$(CC) $(CFLAGS) -g -c $< $(LDFLAGS)

MeshLoader.o: MeshLoader.cpp MeshLoader.hpp
	$(CC) $(CFLAGS) -g -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -g -c $< 

clean:
	/bin/rm -f $(EXEC) *.o debug/*