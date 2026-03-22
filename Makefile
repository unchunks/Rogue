#OBSRC specifies which files to compile as part of the project
OBSRC = src/*/*.cpp

OBJO = 1_Start.o 2_Home.o 3_DungeonMenu.o 4_Dungeon.o 5_Congratulations.o 6_GameOver.o AreaDivide.o AStar.o Character.o Enemy.o Extern.o Game.o Generator.o Log.o LTexture.o main.o Player.o RRA.o Tile.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -I./src

COMPILE_OPTIONS = `sdl2-config --cflags --libs`

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = rogue

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
	rm -f $(OBJO)
	rm -f tests/test_run

valgrind : $(OBSRC)
#$(CC) $(OBSRC) $(COMPILER_FLAGS) -g $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(OBJ_NAME)

debug : $(OBSRC)
	$(CC) $(OBSRC) -g $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)

appimage :
	make release

	# AppDirを削除して再作成
	rm -rf AppDir
	mkdir -p AppDir/usr/bin
	mkdir -p AppDir/usr/lib
	mkdir -p AppDir/usr/share/rogue

	# バイナリをコピー
	cp ./rogue AppDir/usr/bin/

	# アセットをコピー
	cp -r ./assets AppDir/usr/share/rogue/

	# 必要なライブラリを自動収集
	echo "Collecting dependencies..."
	ldd ./rogue | awk '/=>/ {print $$3}' | grep -v '^$$' | while read lib; do \
		if [[ $$lib == /usr/lib/* ]] || [[ $$lib == /lib/* ]]; then \
			echo "Copying $$lib"; \
			cp "$$lib" AppDir/usr/lib/ 2>/dev/null || true; \
		fi \
	done

	# SDL2関連ライブラリを明示的にコピー
	for lib in /usr/lib/x86_64-linux-gnu/libSDL2*.so*; do \
		if [ -f "$$lib" ]; then \
			echo "Copying SDL2 library: $$lib"; \
			cp "$$lib" AppDir/usr/lib/; \
		fi \
	done

	# .desktopファイルを作成
	cat > AppDir/rogue.desktop << EOL
	[Desktop Entry]
	Type=Application
	Name=Rogue
	Exec=rogue
	Icon=rogue
	Categories=Game;
	EOL

	# AppRunスクリプトを作成
	cat > AppDir/AppRun << 'EOL'
	#!/bin/bash
	HERE="$$(dirname "$$(readlink -f "$${0}")")"
	export LD_LIBRARY_PATH="$${HERE}/usr/lib:$${HERE}/usr/lib/x86_64-linux-gnu:$${LD_LIBRARY_PATH}"
	export PATH="$${HERE}/usr/bin:$${PATH}"
	exec "$${HERE}/usr/bin/rogue" "$$@"
	EOL

	chmod +x AppDir/AppRun

	# シンボリックリンクを作成
	ln -sf rogue.desktop AppDir/
	ln -sf usr/share/icons/hicolor/256x256/apps/rogue.png AppDir/ 2>/dev/null || touch AppDir/rogue.png

	echo "AppDir created successfully!"
	echo "Now run: appimagetool AppDir"

	./appimage-builder-x86_64.AppImage --recipe AppImageBuilder.yml

# gdbの手順
#	gdb ./$(BOJS)
#	run
#	bt

test : tests/test_AinB.cpp src/Utils/RectUtils.h
	$(CC) $(COMPILER_FLAGS) -I./tests/include -I./src tests/test_AinB.cpp -o tests/test_run
	./tests/test_run

wc: src/* src/*/*
	wc src/* src/*/* -m -l

play: $(OBSRC)
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev -y
	make release
	./$(OBJ_NAME)

1_Start.o : src/Scene/1_Start.cpp src/Scene/1_Start.h src/UI/Button.h src/Utils/Color.h src/GenerateDungeon/Const.h src/Core/Game.h
	g++ src/Scene/1_Start.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
2_Home.o : src/Scene/2_Home.cpp src/Scene/2_Home.h src/UI/Button.h src/Utils/Color.h src/Utils/Util.h src/GenerateDungeon/Const.h src/System/LTexture.h src/Core/Game.h
	g++ src/Scene/2_Home.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
3_DungeonMenu.o : src/Scene/3_DungeonMenu.cpp src/Scene/3_DungeonMenu.h src/UI/Button.h src/Utils/Color.h src/GenerateDungeon/Const.h src/Core/Game.h
	g++ src/Scene/3_DungeonMenu.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
4_Dungeon.o : src/Scene/4_Dungeon.cpp src/Scene/4_Dungeon.h src/UI/Log.h src/Utils/Color.h src/Utils/Util.h src/GenerateDungeon/Const.h src/GenerateDungeon/AreaDivide.h src/GenerateDungeon/RRA.h src/Characters/Player.h src/Characters/Enemy.h src/GenerateDungeon/Const.h src/GenerateDungeon/Tile.h src/System/LTexture.h src/Core/Game.h
	g++ src/Scene/4_Dungeon.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
5_Congratulations.o : src/Scene/5_Congratulations.cpp src/Scene/5_Congratulations.h src/UI/Button.h src/Utils/Color.h src/GenerateDungeon/Const.h src/Core/Game.h
	g++ src/Scene/5_Congratulations.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
6_GameOver.o : src/Scene/6_GameOver.cpp src/Scene/6_GameOver.h src/UI/Button.h src/Utils/Color.h src/GenerateDungeon/Const.h src/Core/Game.h
	g++ src/Scene/6_GameOver.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
AreaDivide.o : src/GenerateDungeon/AreaDivide.cpp src/GenerateDungeon/AreaDivide.h src/GenerateDungeon/Generator.h
	g++ src/GenerateDungeon/AreaDivide.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
AStar.o : src/System/AStar.cpp src/System/AStar.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h src/Utils/Ivec2.h
	g++ src/System/AStar.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Character.o : src/Characters/Character.cpp src/Characters/Character.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Tile.h src/Utils/Ivec2.h
	g++ src/Characters/Character.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Enemy.o : src/Characters/Enemy.cpp src/Characters/Enemy.h src/System/AStar.h src/Characters/Character.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Tile.h src/Utils/Ivec2.h
	g++ src/Characters/Enemy.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Extern.o : src/Core/Extern.cpp
	g++ src/Core/Extern.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Game.o : src/Core/Game.cpp src/Core/Game.h src/Scene/1_Start.h src/Scene/2_Home.h src/Scene/3_DungeonMenu.h src/Scene/4_Dungeon.h src/Scene/5_Congratulations.h src/GenerateDungeon/Const.h src/Utils/Util.h
	g++ src/Core/Game.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Generator.o : src/GenerateDungeon/Generator.cpp src/GenerateDungeon/Generator.h src/GenerateDungeon/Area.h src/GenerateDungeon/Room.h src/Characters/Player.h src/Characters/Enemy.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Const.h src/Utils/Ivec2.h
	g++ src/GenerateDungeon/Generator.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Log.o : src/UI/Log.cpp src/UI/Log.h src/GenerateDungeon/Const.h src/Utils/Util.h
	g++ src/UI/Log.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
LTexture.o : src/System/LTexture.cpp src/System/LTexture.h src/GenerateDungeon/Const.h src/GenerateDungeon/Enum.h src/Utils/Ivec2.h
	g++ src/System/LTexture.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
main.o : src/Core/main.cpp
	g++ src/Core/main.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Player.o : src/Characters/Player.cpp src/Characters/Player.h src/Characters/Character.h
	g++ src/Characters/Player.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
RRA.o : src/GenerateDungeon/RRA.cpp src/GenerateDungeon/RRA.h src/GenerateDungeon/Generator.h
	g++ src/GenerateDungeon/RRA.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
Tile.o : src/GenerateDungeon/Tile.cpp src/GenerateDungeon/Tile.h src/GenerateDungeon/Enum.h src/GenerateDungeon/Const.h src/System/LTexture.h
	g++ src/GenerateDungeon/Tile.cpp -c $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS)
