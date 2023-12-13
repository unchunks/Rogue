#include "Scene/6_GameOver.h"

extern SDL_Renderer *gRenderer;
extern TTF_Font *gFontN, *gFontB;
extern Mix_Chunk* gClickEffect;
extern SCENE gNowScene;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

void toHomeOver()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::HOME;
    // SDL_Log("toHomeOver\n");
}
void toDungeonmenuOver()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::DUNGEON_MENU;
    // SDL_Log("toDungeonmenuOver\n");
}

GameOver::GameOver()
{
    LoadData();

    to_home.onClick = &toHomeOver;
    to_dungeonmenu.onClick = &toDungeonmenuOver;
    to_home.create(
        gRenderer,
        (SCREEN_WIDTH / 2 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H + 200) / 2,
        BUTTON_W,
        BUTTON_H,
        "1 :　　 ホーム画面へ　　",
        gFontN,
        Color::SDL_blue,
        Color::SDL_white);
    to_dungeonmenu.create(
        gRenderer,
        (SCREEN_WIDTH * 1.5 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H + 200) / 2,
        BUTTON_W,
        BUTTON_H,
        "2 : ダンジョンメニューへ",
        gFontN,
        Color::SDL_blue,
        Color::SDL_white);
}

GameOver::~GameOver()
{
}

void GameOver::LoadData()
{
}

void GameOver::Input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_1: to_home.press();        break;
        case SDLK_2: to_dungeonmenu.press(); break;
        }
        return;
    }

    switch(event.key.keysym.sym)
    {
    case SDLK_1:
        if(to_home.isPressed)
        {
            to_home.release(); 
            to_home.onClick();
        }
        break;
    case SDLK_2:
        if(to_dungeonmenu.isPressed)
        {
            to_dungeonmenu.release(); 
            to_dungeonmenu.onClick();
        }
        break;
    }
}

void GameOver::Update()
{

}

void GameOver::Output()
{
    SDL_SetRenderDrawColor(gRenderer, 255, 45, 45, 255);
    SDL_RenderFillRect(gRenderer, &over_back);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(gFontB, "GameOver", {255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_RenderCopy(gRenderer, texture, NULL, &over_text);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    to_home.Draw();
    to_dungeonmenu.Draw();
}

