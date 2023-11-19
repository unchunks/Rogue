#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "src/UI/Button.h"
SDL_atomic_t frames;

int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 400;

/* 設定された間隔で平均フレームレートの計算と表示を行う */
Uint32 fps_timer_callback(Uint32 interval, void *data)
{
	const float f = SDL_AtomicGet(&frames);
	const float iv = interval * 0.001f;

	/* 注意: printfがスレッドセーフであるかは環境に依存する */
	printf("%.2f\tfps\n", f / iv);

	/* フレームカウンタをリセットする */
	SDL_AtomicSet(&frames, 0);

	return interval;
}

int main(int argc, char **argv)
{
	// Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (sdlResult != 0)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_IMG
    sdlResult = IMG_Init(IMG_INIT_PNG);
    if (!(sdlResult & IMG_INIT_PNG))
    {
        printf("Unable to initialize SDL_IMG: %s", IMG_GetError());
        return false;
    }

    // Initialize SDL_TTF
    sdlResult = TTF_Init();
    if (sdlResult != 0)
    {
        printf("Unable to initialize SDL_TTF: %s", TTF_GetError());
        return false;
    }

    TTF_Font *mFont = TTF_OpenFont("assets/JF-Dot-K14B.ttf", 30);
    if (!mFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }

    // Initialize SDL_Mix
    sdlResult = Mix_Init(MIX_INIT_MP3);
    if (sdlResult < 0)
    {
        printf("Unable to initialize SDL_Mix: %s", Mix_GetError());
        return false;
    }
    
    sdlResult = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    if (sdlResult < 0)
    {
        printf("Unable to initialize SDL_mixer: %s", Mix_GetError());
        return false;
    }

    // Create an SDL Window
    SDL_Window *win = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100,                                   // Top left x-coordinate of window
        100,                                   // Top left y-coordinate of window
        SCREEN_WIDTH,                              // Width of window
        SCREEN_HEIGHT,                              // Height of window
        0                                      // Flags (0 for no flags set)
    );

    if (!win)
    {
        printf("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        win, // Window to create renderer for
        -1,      // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

	/* タイマは別スレッドで5秒間隔で動く */
	SDL_AddTimer(1000, fps_timer_callback, NULL);

	Button button;
	button.create(
		renderer,
		SCREEN_WIDTH/2-100,
		SCREEN_HEIGHT/2-100,
	    200,
		200,
		"Test Button",
		mFont,
		Color::SDL_blue,
		Color::SDL_white);

	int beforTime = SDL_GetTicks();
	int afterTime = SDL_GetTicks();
	int elapsedTime = 0;
	SDL_Event e;
	while (1)
	{
		beforTime = SDL_GetTicks();
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				goto quit;
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    	SDL_RenderClear(renderer);

		button.Draw();
		SDL_RenderPresent(renderer);
		

		/* フレーム数に1を加える */
		SDL_AtomicAdd(&frames, 1);
		afterTime = SDL_GetTicks();
		elapsedTime = afterTime - beforTime;
		if (33 > elapsedTime)
		{
			SDL_Delay(33 - elapsedTime);
		}
	}
quit:
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}