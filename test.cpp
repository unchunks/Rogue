#include <SDL2/SDL.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDLの初期化に失敗しました: %s", SDL_GetError());
        return 1;
    }

    // ウィンドウの作成時にSDL_WINDOW_ALLOW_HIGHDPIフラグとSDL_WINDOW_OPENGLを追加
    SDL_Window* window = SDL_CreateWindow("Transparent Rectangle",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window) {
        SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // ウィンドウ表示
    SDL_ShowWindow(window);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("レンダラの作成に失敗しました: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 透明な背景色 (RGBA)
    Uint8 r = 255;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 128; // 透明度

    // 透明な四角形の描画
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_RenderFillRect(renderer, &rect);

    // ウィンドウ表示
    SDL_RenderPresent(renderer);

    // イベントループ
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }

    // 後片付け
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
