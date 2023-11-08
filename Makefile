#OBJS specifies which files to compile as part of the project
OBJS = src/*.cpp

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
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)

do : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
	./$(OBJ_NAME)

release : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -O3 -s $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -g $(COMPILE_OPTIONS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
	valgrind --track-origins=yes --leak-check=full ./$(OBJ_NAME)

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

# real    0m18.780s
# user    0m15.831s
# sys     0m2.673s