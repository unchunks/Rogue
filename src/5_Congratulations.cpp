#include "Scene/5_Congratulations.h"

extern SDL_Renderer *gRenderer;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

Game *cong_g;
void toHome()
{
    Mix_PlayChannel(-1, cong_g->getClickEffect(), 0);
    cong_g->setNowScene(SCENE::HOME);
    printf("toHome\n");
}
void toDungeonmenu()
{
    Mix_PlayChannel(-1, cong_g->getClickEffect(), 0);
    cong_g->setNowScene(SCENE::DUNGEON_MENU);
    printf("toDungeonmenu\n");
}

Congratulations::Congratulations(Game *game)
{
    cong_g = game;
    mGame = game;
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
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    to_dungeonmenu.create(
        gRenderer,
        (SCREEN_WIDTH * 1.5 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H + 200) / 2,
        BUTTON_W,
        BUTTON_H,
        "2 : ダンジョンメニューへ",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
}

Congratulations::~Congratulations()
{
}

void Congratulations::LoadData()
{
    fontB = TTF_OpenFont("assets/JF-Dot-K14B.ttf", 30 - 1);
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
    }
    else
    {
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
}

void Congratulations::Update()
{

}

void Congratulations::Output()
{
    SDL_SetRenderDrawColor(gRenderer, 255, 238, 58, 255);
    SDL_RenderFillRect(gRenderer, &cong_back);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(fontB, "Congratulations!", {255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_RenderCopy(gRenderer, texture, NULL, &cong_text);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    to_home.Draw();
    to_dungeonmenu.Draw();
}

