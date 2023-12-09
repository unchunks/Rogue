#OBSRC specifies which files to compile as part of the project
OBSRC = src/*.cpp

OBJO = 1_Start.o 2_Home.o 3_DungeonMenu.o 4_Dungeon.o AreaDivide.o AStar.o Character.o Enemy.o Extern.o Game.o Generator.o Log.o LTexture.o main.o OSearch.o Player.o RRA.o Tile.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall

COMPILE_OPTIONS = `sdl2-config --cflags --libs`

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game

#This is the target that compiles our executable
link : 1_Start.o 2_Home.o 3_DungeonMenu.o 4_Dungeon.o AreaDivide.o AStar.o Character.o Enemy.o Extern.o Game.o Generator.o Log.o LTexture.o main.o OSearch.o Player.o RRA.o Tile.o
	$(CC) 1_Start.o 2_Home.o 3_DungeonMenu.o 4_Dungeon.o AreaDivide.o AStar.o Character.o Enemy.o Extern.o Game.o Generator.o Log.o LTexture.o main.o OSearch.o Player.o RRA.o Tile.o -o $(OBJ_NAME) $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)

compile : 1_Start.o 2_Home.o 3_DungeonMenu.o 4_Dungeon.o AreaDivide.o AStar.o Character.o Enemy.o Extern.o Game.o Generator.o Log.o LTexture.o main.o OSearch.o Player.o RRA.o Tile.o

do : $(OBJO)
	make link
	./$(OBJ_NAME)

release : $(OBSRC)
	$(CC) $(OBSRC) $(COMPILER_FLAGS) -O3 -s $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)

valgrind : $(OBSRC)
#$(CC) $(OBSRC) $(COMPILER_FLAGS) -g $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(OBJ_NAME)

gdb: $(BOJS)
	gdb ./$(BOJS)
	run
	bt
	frame 0

test : test.cpp
	$(CC) test.cpp $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o test
	./test

commit-% :
	git add -A
	git commit -m "${@:commit-%=%}"

push-% :
	git add -A
	git commit -m "${@:push-%=%}"
	git push

wc: *.cpp *.h src/* src/*/*
	wc *.cpp *.h src/* src/*/* -m -l

play: $(OBSRC)
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev -y
	make do

1_Start.o : src/1_Start.cpp src/Scene/1_Start.h
	g++ src/1_Start.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
2_Home.o : src/2_Home.cpp src/Scene/2_Home.h
	g++ src/2_Home.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
3_DungeonMenu.o : src/3_DungeonMenu.cpp src/Scene/3_DungeonMenu.h
	g++ src/3_DungeonMenu.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
4_Dungeon.o : src/4_Dungeon.cpp src/Scene/4_Dungeon.h
	g++ src/4_Dungeon.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
AreaDivide.o : src/AreaDivide.cpp src/GenerateDungeon/AreaDivide.h
	g++ src/AreaDivide.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
AStar.o : src/AStar.cpp src/GenerateDungeon/AStar.h
	g++ src/AStar.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Character.o : src/Character.cpp src/GenerateDungeon/Character.h
	g++ src/Character.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Enemy.o : src/Enemy.cpp src/GenerateDungeon/Enemy.h
	g++ src/Enemy.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Extern.o : src/Extern.cpp
	g++ src/Extern.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Game.o : src/Game.cpp src/Game.h
	g++ src/Game.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Generator.o : src/Generator.cpp src/GenerateDungeon/Generator.h
	g++ src/Generator.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Log.o : src/Log.cpp src/UI/Log.h
	g++ src/Log.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
LTexture.o : src/LTexture.cpp src/GenerateDungeon/LTexture.h
	g++ src/LTexture.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
main.o : src/main.cpp
	g++ src/main.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
OSearch.o : src/OSearch.cpp src/GenerateDungeon/OSearch.h
	g++ src/OSearch.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Player.o : src/Player.cpp src/GenerateDungeon/Player.h
	g++ src/Player.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
RRA.o : src/RRA.cpp src/GenerateDungeon/RRA.h
	g++ src/RRA.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Tile.o : src/Tile.cpp src/GenerateDungeon/Tile.h
	g++ src/Tile.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
