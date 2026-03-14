#ifndef SDL_H
#define SDL_H

typedef struct SDL_Rect {
    int x, y;
    int w, h;
} SDL_Rect;

typedef struct SDL_Point {
    int x, y;
} SDL_Point;

typedef enum {
    SDL_FLIP_NONE = 0x00000000,
    SDL_FLIP_HORIZONTAL = 0x00000001,
    SDL_FLIP_VERTICAL = 0x00000002
} SDL_RendererFlip;

typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Renderer SDL_Renderer;
typedef enum { SDL_BLENDMODE_NONE = 0 } SDL_BlendMode;
typedef struct { unsigned char r, g, b, a; } SDL_Color;
typedef unsigned char Uint8;

#define SDL_GetError() "Mock error"

#endif
