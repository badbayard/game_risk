# Compilateur
CC = g++

# Options du compilateur
# 	Debug
CCFLAGS = -g -Wall
# 	Creation fichier objet
OBJFLAGS = -c

# Specification norme C++11 pour compiler 
C11FLAG = -std=c++11


# Repertoire bibliotheques externe
INCLUDE_SDL_DIR = -I/usr/include/SDL2

# Modules SDL2 a inclure
LIBS_SDL = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer


OBJ = obj/Combat.o obj/Terrain.o obj/Pays.o obj/Region.o obj/Joueur.o



all : bin/test.out bin/testSDL.out



bin/test.out : obj/main.o obj/Jeu.o $(OBJ)
	$(CC) $(OBJ) obj/main.o obj/Jeu.o -o bin/test.out

bin/testSDL.out : obj/JeuSDL.o obj/mainSDL.o obj/Jeu.o $(OBJ)
	$(CC) $(OBJ) $(INCLUDE_SDL_DIR) $(C11FLAG) obj/JeuSDL.o obj/Jeu.o obj/mainSDL.o -o bin/testSDL.out $(LIBS_SDL)



obj/mainSDL.o : src/mainSDL.cpp src/JeuSDL.h src/JeuSDL.cpp src/Jeu.h src/Jeu.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) $(INCLUDE_SDL_DIR) $(C11FLAG) src/mainSDL.cpp -o obj/mainSDL.o $(LIBS_SDL)

obj/main.o : src/main.cpp src/Jeu.h src/Jeu.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/main.cpp -o obj/main.o

obj/JeuSDL.o : src/JeuSDL.cpp src/JeuSDL.h src/Jeu.h src/Jeu.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) $(INCLUDE_SDL_DIR) $(C11FLAG) src/JeuSDL.cpp -o obj/JeuSDL.o $(LIBS_SDL)

obj/Jeu.o : src/Jeu.cpp src/Jeu.h src/Combat.h src/Joueur.h src/Terrain.h src/Combat.cpp src/Joueur.cpp src/Terrain.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/Jeu.cpp -o obj/Jeu.o

obj/Combat.o : src/Combat.cpp src/Combat.h src/Joueur.h src/Region.h src/Joueur.cpp src/Region.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/Combat.cpp -o obj/Combat.o

obj/Terrain.o : src/Terrain.cpp src/Terrain.h src/Pays.h src/Region.h src/Pays.cpp src/Region.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/Terrain.cpp -o obj/Terrain.o

obj/Pays.o : src/Pays.cpp src/Pays.h src/Region.cpp
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/Pays.cpp -o obj/Pays.o

obj/Region.o : src/Region.cpp src/Region.h
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/Region.cpp -o obj/Region.o

obj/Joueur.o : src/Joueur.cpp src/Joueur.h
	$(CC) $(CCFLAGS) $(OBJFLAGS) src/Joueur.cpp -o obj/Joueur.o



clean :
	rm obj/*.o
