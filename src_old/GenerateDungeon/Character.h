#pragma once

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>
#include <SDL.h>

#include "Enum.h"
#include "Const.h"
#include "Tile.h"

enum STATE {
    // プレイヤー
    ALIVE,
    // 敵キャラ
    SEARCH,
    FOUND,
    ESCAPE,
    // どちらも
    DEAD
};

class Character
{
public:
    Character();
    Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type);

    void move(DIRECTION _dir);
    void moveTo(glm::vec2 _pos);
    void back();
    virtual void attack(class Character& _enemy);
    void receiveDamage(int _damage);

    void setPos(int _x, int _y);
    void setPos(glm::vec2 _pos);
    void setState(STATE _state);
    void setDir(DIRECTION _dir);

    int getNowHP() {return nowHP;}
    STATE getState() {return mState;}
    DIRECTION getDir() {return mDir;}
    CHAR_TYPE getType() {return mType;}
    glm::vec2 getPos() {return mPos;}

    //TODO: キー入力の取得と速度の調整
    void handleEvent( SDL_Event& e );

    //プレイヤー移動とタイルとの衝突判定
    void move( Tile *tiles[] );

    //カメラの中心をプレイヤーに合わせる
    void setCamera( SDL_Rect& camera );

    //プレイヤーをスクリーンに表示
    void render( SDL_Rect& camera );

    //プレイヤーの衝突判定
    SDL_Rect mBox;

    //プレイヤーの速度
    int mVelX, mVelY;

    SDL_Rect gSpriteClips[ ANIMATION_FRAMES * NO_DIRECTION ];

protected:
    int maxHP, STR, VIT;
    int nowHP;
    STATE mState;
    DIRECTION mDir;
    CHAR_TYPE mType;
    glm::vec2 mPos;
};
