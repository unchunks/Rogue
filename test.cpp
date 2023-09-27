/* これはFPSをカウントするサンプルである
 *
 * SDLタイマは別のスレッドで動いている
 * そのためデータ競合を防ぐ必要である
 */
#include <SDL.h>
SDL_atomic_t frames;

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
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Window *win = SDL_CreateWindow("Counter",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        640,
                        480,
                        0
                        );

        SDL_Surface *screen = SDL_GetWindowSurface(win);

        /* タイマは別スレッドで5秒間隔で動く */
        SDL_AddTimer(1000, fps_timer_callback, NULL);

        int beforTime = SDL_GetTicks();
        int afterTime = SDL_GetTicks();
        int elapsedTime = 0;
        SDL_PixelFormat *fmt;
        fmt = screen->format;
        SDL_Event e;
        while(1) {
            beforTime = SDL_GetTicks();
                while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT) {
                                goto quit;
                        }
                }
                SDL_FillRect(screen, NULL, SDL_MapRGB(fmt, 0, 50, 0));
                SDL_UpdateWindowSurface(win);
                

                /* フレーム数に1を加える */
                SDL_AtomicAdd(&frames, 1);
            afterTime = SDL_GetTicks();
            elapsedTime = afterTime - beforTime;
            if(33 > elapsedTime) {
                SDL_Delay(33 - elapsedTime);
            }
        }
quit:
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 0;
}