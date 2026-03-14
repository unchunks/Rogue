#ifndef SDL_IMAGE_H
#define SDL_IMAGE_H

#include "SDL.h"

typedef struct SDL_Surface {
    int w, h;
    void* pixels;
} SDL_Surface;

inline SDL_Surface* IMG_Load(const char* file) { return (SDL_Surface*)0; }
inline void SDL_FreeSurface(SDL_Surface* surface) {}
inline SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) { return (SDL_Texture*)0; }

#endif
