#OBSRC specifies which files to compile as part of the project
OBSRC = src/*.cpp

OBJO = 1_Start.o 2_Home.o 3_DungeonMenu.o 4_Dungeon.o 5_Congratulations.o 6_GameOver.o AreaDivide.o AStar.o Character.o Enemy.o Extern.o Game.o Generator.o Log.o LTexture.o main.o Player.o RRA.o Tile.o

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
link : $(OBJO)
	$(CC) $(OBJO) -o $(OBJ_NAME) $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)

compile : $(OBJO)

reset : $(OBJO)
	make clean
	make link

do : $(OBJO)
	make link
	./$(OBJ_NAME)

release : $(OBSRC)
	$(CC) $(OBSRC) $(COMPILER_FLAGS) -O3 -s $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)

.PHONY : clean
clean : $(OBJO)
	rm $(OBJO)

valgrind : $(OBSRC)
#$(CC) $(OBSRC) $(COMPILER_FLAGS) -g $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(OBJ_NAME)

debug : $(OBSRC)
	$(CC) $(OBSRC) -g $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)

# gdbの手順
#	gdb ./$(BOJS)
#	run
#	bt
#	frame 0

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

1_Start.o : src/1_Start.cpp src/Scene/1_Start.h src/UI/Button.h src/Functions/Color.h src/GenerateDungeon/Const.h src/Game.h
	g++ src/1_Start.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
2_Home.o : src/2_Home.cpp src/Scene/2_Home.h src/UI/Button.h src/Functions/Color.h src/GenerateDungeon/Const.h src/GenerateDungeon/LTexture.h src/Game.h
	g++ src/2_Home.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
3_DungeonMenu.o : src/3_DungeonMenu.cpp src/Scene/3_DungeonMenu.h src/UI/Button.h src/Functions/Color.h src/GenerateDungeon/Const.h src/Game.h
	g++ src/3_DungeonMenu.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
4_Dungeon.o : src/4_Dungeon.cpp src/Scene/4_Dungeon.h src/UI/Log.h src/Functions/Color.h src/GenerateDungeon/Const.h src/GenerateDungeon/AreaDivide.h src/GenerateDungeon/RRA.h src/GenerateDungeon/Player.h src/GenerateDungeon/Enemy.h src/GenerateDungeon/Const.h src/GenerateDungeon/Tile.h src/GenerateDungeon/LTexture.h src/Game.h
	g++ src/4_Dungeon.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
5_Congratulations.o : src/5_Congratulations.cpp src/Scene/5_Congratulations.h src/UI/Button.h src/Functions/Color.h src/GenerateDungeon/Const.h src/Game.h
	g++ src/5_Congratulations.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
6_GameOver.o : src/6_GameOver.cpp src/Scene/6_GameOver.h src/UI/Button.h src/Functions/Color.h src/GenerateDungeon/Const.h src/Game.h
	g++ src/6_GameOver.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
AreaDivide.o : src/AreaDivide.cpp src/GenerateDungeon/AreaDivide.h src/GenerateDungeon/Generator.h
	g++ src/AreaDivide.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
AStar.o : src/AStar.cpp src/GenerateDungeon/AStar.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h Ivec2.h 
	g++ src/AStar.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Character.o : src/Character.cpp src/GenerateDungeon/Character.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Tile.h Ivec2.h
	g++ src/Character.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Enemy.o : src/Enemy.cpp src/GenerateDungeon/Enemy.h src/GenerateDungeon/AStar.h src/GenerateDungeon/Character.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Tile.h Ivec2.h
	g++ src/Enemy.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Extern.o : src/Extern.cpp
	g++ src/Extern.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Game.o : src/Game.cpp src/Game.h src/Scene/1_Start.h src/Scene/2_Home.h src/Scene/3_DungeonMenu.h src/Scene/4_Dungeon.h src/Scene/5_Congratulations.h
	g++ src/Game.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Generator.o : src/Generator.cpp src/GenerateDungeon/Generator.h src/GenerateDungeon/Area.h src/GenerateDungeon/Room.h src/GenerateDungeon/Player.h src/GenerateDungeon/Enemy.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Const.h Ivec2.h
	g++ src/Generator.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Log.o : src/Log.cpp src/UI/Log.h src/GenerateDungeon/Const.h
	g++ src/Log.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
LTexture.o : src/LTexture.cpp src/GenerateDungeon/LTexture.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h Ivec2.h
	g++ src/LTexture.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
main.o : src/main.cpp
	g++ src/main.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Player.o : src/Player.cpp src/GenerateDungeon/Player.h src/GenerateDungeon/Character.h
	g++ src/Player.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
RRA.o : src/RRA.cpp src/GenerateDungeon/RRA.h src/GenerateDungeon/Generator.h
	g++ src/RRA.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Tile.o : src/Tile.cpp src/GenerateDungeon/Tile.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Const.h src/GenerateDungeon/LTexture.h
	g++ src/Tile.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
