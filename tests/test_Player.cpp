#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <stdarg.h>

#include "SDL2/SDL.h"
#include "GenerateDungeon/Player.h"
#include "GenerateDungeon/Character.h"

// Stubs for global/static variables needed by Character.cpp and Tile.cpp
SDL_Renderer* gRenderer = nullptr;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

// Stubs for missing SDL functions
void SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {}
void SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect) {}
void SDL_Log(const char* fmt, ...) {}
int SDL_SetColorKey(SDL_Surface* surface, int flag, int key) { return 0; }
int SDL_MapRGB(const void* format, Uint8 r, Uint8 g, Uint8 b) { return 0; }
void SDL_DestroyTexture(SDL_Texture* texture) {}
int SDL_SetTextureColorMod(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b) { return 0; }
int SDL_SetTextureBlendMode(SDL_Texture* texture, SDL_BlendMode blendMode) { return 0; }
int SDL_SetTextureAlphaMod(SDL_Texture* texture, Uint8 alpha) { return 0; }
int SDL_RenderCopyEx(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip) { return 0; }

void test_player_reset() {
    std::cout << "Running test_player_reset..." << std::endl;

    // Initial setup: maxHP=100, STR=20, VIT=20
    Player p(10, 20, 100, 20, 20);

    // 1. Modify states to non-default values
    p.receiveDamage(50); // nowHP becomes 60 (100 - (50 - 20/2))
    assert(p.getNowHP() == 60);

    p.levelUp(100); // level becomes 2, exp becomes 0, maxHP += 10 (110), nowHP += 10 (70)
    assert(p.level == 2);
    assert(p.getMaxHP() == 110);
    assert(p.getNowHP() == 70);

    p.setDir(LEFT);
    p.setState(DEAD);

    // 2. Call reset
    p.reset();

    // 3. Verify reset states
    // nowHP should be reset to current maxHP
    assert(p.getNowHP() == p.getMaxHP());
    assert(p.getNowHP() == 110); // Note: current implementation doesn't reset maxHP to initial

    // mState should be ALIVE
    assert(p.getState() == ALIVE);

    // mDir should be DOWN
    assert(p.getDir() == DOWN);

    // level should be 1
    assert(p.level == 1);

    // exp should be 0
    assert(p.exp == 0);

    std::cout << "test_player_reset passed!" << std::endl;
}

int main() {
    test_player_reset();
    std::cout << "All Player tests passed!" << std::endl;
    return 0;
}
