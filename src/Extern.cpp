#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "GenerateDungeon/Enum.h"

SDL_Window *gWindow;
SDL_Renderer *gRenderer;

TTF_Font *gFontN, *gFontB;

Mix_Chunk* gClickEffect;

SCENE gNowScene;