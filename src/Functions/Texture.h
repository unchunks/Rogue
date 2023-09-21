#ifndef __TEXTURE
#define __TEXTURE

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer) {
    //The return texture
    SDL_Texture* texture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
        return texture;
    }
    //Create texture form surface pixels
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture == NULL) {
        printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
        return texture;
    }
    SDL_FreeSurface(loadedSurface);
    return texture;
}

#endif __TEXTURE