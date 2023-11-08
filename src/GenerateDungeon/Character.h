#pragma once

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>

#include "Enum.h"
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
    ~Character();

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
    glm::vec2 getPos() {return glm::vec2(mBox.x, mBox.y);}

    bool onTileCenter();

    bool move(std::vector<class Tile> _tiles, std::vector<class Character> _otherCharacters);
    void setCamera(SDL_Rect& _camera);
    void render(SDL_Rect& _camera);

    static LTexture mCharTexture;
    // LTexture mCharTexture;

    std::vector<SDL_Rect> mSpriteClips;

    bool isMoved;

protected:
    bool touchWall(SDL_Rect box, std::vector<Tile> _tiles);
    bool touchChar(std::vector<class Character> _otherCharacters);
    bool mapOver();

    int nowHP;
    int maxHP, STR, VIT;
    STATE mState;
    DIRECTION mDir;
    CHAR_TYPE mType;

    //プレイヤーの衝突判定
    SDL_Rect mBox;

    int mAnimFrame;
};
