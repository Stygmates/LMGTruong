CC = g++
CFLAGS = -O2 -Wall
LDFLAGS = -lGL -lglut -lGLEW -Iglm -lSOIL -lassimp -Iassimp -I /usr/include/SOIL
EXEC = main
OBJ = $(SRC:.c=.o)

all: $(OBJ) Loader.o My3DModel.o 
	$(CC) -std=c++11 -o $(EXEC) exo1.cpp $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -g -c $< $(CFLAGS)

clean:
	/bin/rm -f $(EXEC) *.o main