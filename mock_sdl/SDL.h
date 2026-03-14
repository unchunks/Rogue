#ifndef MOCK_SDL_H
#define MOCK_SDL_H
#include <stdint.h>
typedef uint8_t Uint8;
typedef uint32_t Uint32;
typedef struct SDL_Rect { int x, y, w, h; } SDL_Rect;
typedef struct SDL_Color { Uint8 r, g, b, a; } SDL_Color;
typedef void SDL_Renderer;
typedef void SDL_Texture;
typedef void SDL_Window;
typedef void SDL_Point;
typedef int SDL_RendererFlip;
typedef int SDL_BlendMode;
#define SDL_FLIP_NONE 0
#define SDL_Log(...)
#endif
