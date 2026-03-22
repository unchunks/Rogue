#include <iostream>
#include <cassert>
#include "src/Characters/Character.h"
#include <SDL.h>

// Stubs for global/static variables needed by Character.cpp and Tile.cpp
SDL_Renderer* gRenderer = nullptr;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

// We need a dummy for LTexture methods used in Character.cpp and Tile.cpp
void LTexture::render( int x, int y, SDL_Rect* clip, bool filter, double angle, SDL_Point* center, SDL_RendererFlip flip ) {}
LTexture::LTexture() : mTexture(nullptr), mWidth(0), mHeight(0) {}
LTexture::~LTexture() {}

void test_partial_healing() {
    // x, y, maxHP, STR, VIT, state, dir, type, name
    Character c(0, 0, 100, 10, 10, ALIVE, DOWN, PLAYER, "TestHero");

    // nowHP starts at 100. reduce it.
    // receiveDamage(55) -> 55 - (10/2) = 50 damage. nowHP = 50.
    c.receiveDamage(55);
    assert(c.getNowHP() == 50);

    c.healed(30);
    assert(c.getNowHP() == 80);
    std::cout << "test_partial_healing passed!" << std::endl;
}

void test_full_healing() {
    Character c(0, 0, 100, 10, 10, ALIVE, DOWN, PLAYER, "TestHero");
    c.receiveDamage(55); // nowHP = 50

    c.healed(50);
    assert(c.getNowHP() == 100);
    std::cout << "test_full_healing passed!" << std::endl;
}

void test_overflow_healing() {
    Character c(0, 0, 100, 10, 10, ALIVE, DOWN, PLAYER, "TestHero");
    c.receiveDamage(55); // nowHP = 50

    c.healed(100);
    // Introduce an artificial bug in the test assertion to confirm it can fail
    // assert(c.getNowHP() == 150); // This should fail
    assert(c.getNowHP() == 100);
    std::cout << "test_overflow_healing passed!" << std::endl;
}

void test_healing_at_max_hp() {
    Character c(0, 0, 100, 10, 10, ALIVE, DOWN, PLAYER, "TestHero");
    assert(c.getNowHP() == 100);

    c.healed(10);
    assert(c.getNowHP() == 100);
    std::cout << "test_healing_at_max_hp passed!" << std::endl;
}

void test_healing_with_zero() {
    Character c(0, 0, 100, 10, 10, ALIVE, DOWN, PLAYER, "TestHero");
    c.receiveDamage(55); // nowHP = 50

    c.healed(0);
    assert(c.getNowHP() == 50);
    std::cout << "test_healing_with_zero passed!" << std::endl;
}

int main(int argc, char* argv[]) {
    test_partial_healing();
    test_full_healing();
    test_overflow_healing();
    test_healing_at_max_hp();
    test_healing_with_zero();

    std::cout << "All tests passed using real Character class!" << std::endl;
    return 0;
}
