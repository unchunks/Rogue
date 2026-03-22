#include "Scene/3_DungeonMenu.h"

extern SDL_Renderer *gRenderer;
extern TTF_Font *gFontN;
extern Mix_Chunk* gClickEffect;
extern SCENE gNowScene;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

void area_divide()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::DUNGEON_AREA_DIVIDE;
    // SDL_Log("area_divide\n");
}
void RRA()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::DUNGEON_RRA;
    // SDL_Log("RRA\n");
}

DungeonMenu::DungeonMenu()
{
    LoadData();

    mAreaDivide.onClick = &area_divide;
    mRRA.onClick = &RRA;
    mAreaDivide.create(
        gRenderer,
        (SCREEN_WIDTH / 2 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "1 : 区域分割法",
        gFontN,
        Color::SDL_blue,
        Color::SDL_white);
    mRRA.create(
        gRenderer,
        (SCREEN_WIDTH * 1.5 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "2 : 　 RRA 　",
        gFontN,
        Color::SDL_blue,
        Color::SDL_white);

    
}

DungeonMenu::~DungeonMenu()
{
}

void DungeonMenu::LoadData()
{
}

void DungeonMenu::Input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_1:
            mAreaDivide.press();
            break;
        case SDLK_2:
            mRRA.press();
            break;
        }
        return;
    }
    switch(event.key.keysym.sym)
    {
    case SDLK_1:
        if(mAreaDivide.isPressed)
        {
            mAreaDivide.release();
            mAreaDivide.onClick();
        }
        break;
    case SDLK_2:
        if(mRRA.isPressed)
        {
            mRRA.release();
            mRRA.onClick();
        }
        break;
    }
}

void DungeonMenu::Update()
{

}

void DungeonMenu::Output()
{
    mAreaDivide.Draw();
    mRRA.Draw();
}

void DungeonMenu::PlayMusic()
{
}
