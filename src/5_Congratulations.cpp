#include "Scene/5_Congratulations.h"

extern SDL_Renderer *gRenderer;
extern TTF_Font *gFontN, *gFontB;
extern Mix_Chunk* gClickEffect;
extern SCENE gNowScene;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

void toHome()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::HOME;
    printf("toHome\n");
}
void toDungeonmenu()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::DUNGEON_MENU;
    printf("toDungeonmenu\n");
}

Congratulations::Congratulations()
{
    LoadData();

    to_home.onClick = &toHome;
    to_dungeonmenu.onClick = &toDungeonmenu;
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

Congratulations::~Congratulations()
{
}

void Congratulations::LoadData()
{
}

void Congratulations::Input(SDL_Event event)
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

void Congratulations::Update()
{

}

void Congratulations::Output()
{
    SDL_SetRenderDrawColor(gRenderer, 255, 238, 58, 255);
    SDL_RenderFillRect(gRenderer, &cong_back);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(gFontB, "Congratulations!", {255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_RenderCopy(gRenderer, texture, NULL, &cong_text);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    to_home.Draw();
    to_dungeonmenu.Draw();
}

