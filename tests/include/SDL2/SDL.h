#ifndef SDL_H
#define SDL_H

#include <stdarg.h>

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

typedef struct SDL_Surface {
    int w, h;
    void* pixels;
    struct { int format; }* format;
} SDL_Surface;

#define SDL_TRUE 1
#define SDL_FALSE 0

#define SDL_GetError() "Mock error"
#define IMG_GetError() "Mock IMG error"

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect);
void SDL_Log(const char* fmt, ...);
int SDL_SetColorKey(SDL_Surface* surface, int flag, int key);
int SDL_MapRGB(const void* format, Uint8 r, Uint8 g, Uint8 b);
void SDL_DestroyTexture(SDL_Texture* texture);
int SDL_SetTextureColorMod(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b);
int SDL_SetTextureBlendMode(SDL_Texture* texture, SDL_BlendMode blendMode);
int SDL_SetTextureAlphaMod(SDL_Texture* texture, Uint8 alpha);
int SDL_RenderCopyEx(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip);

inline SDL_Surface* IMG_Load(const char* file) { return (SDL_Surface*)0; }
inline void SDL_FreeSurface(SDL_Surface* surface) {}
inline SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) { return (SDL_Texture*)0; }

#endif
