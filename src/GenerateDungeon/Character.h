#pragma once

#include <vector>
#include <cstdlib>
#include <string>
#include "../../Ivec2.h"

#include "Enum.h"
#include "Tile.h"
#include "Const.h"

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
    Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type, std::string _name);
    ~Character();

    DIRECTION adjacent(class Character _opponent);
	DIRECTION adjacent(std::vector<class Character> _opponents);
    virtual std::string attack(class Character& _opponent);
    std::string receiveDamage(int _damage);
    void healed(int _heal_val);

    /// @brief 座標テレポート
    /// @param _image_pos 画像系座標
    void setImagePos(Ivec2 _image_pos);

    /// @brief 座標テレポート
    /// @param _data_pos データ系座標
    void setDataPos(Ivec2 _data_pos);
    void setState(STATE _state);
    void setDir(DIRECTION _dir);

    int getNowHP() const {return nowHP;}
    int getMaxHP() const {return maxHP;}
    int getSTR() const {return STR;}
    int getVIT() const {return VIT;}
    STATE getState() const {return mState;}
    DIRECTION getDir() const {return mDir;}
    CHAR_TYPE getType() const {return mType;}
    std::string getName() const {return mName;}

    /// @brief 画像系座標を返す
    Ivec2 getImagePos() const {return Ivec2(mBox.x, mBox.y);}

	/// @brief データ系座標を返す
    Ivec2 getDataPos() const {return Ivec2(static_cast<int>(mBox.x / TILE_W), static_cast<int>(mBox.y / TILE_H));}
	
	/// @brief 前方の座標を返す
    /// @return NO_DIRECTIONの場合は現在地を返す
	Ivec2 getFrontDataPos();
    bool onTileCenter();

    /// @brief 向いている方向に移動。当たり判定も含む
    /// @param _tiles 当たり判定用の全タイルの配列
    bool move(std::vector<class Tile> _tiles, const std::vector<class Character>& _otherCharacters);
    
    /// @brief 向いている方向に移動。当たり判定も含む
    /// @param _destination 移動先のデータ系座標
    /// @param _tiles 当たり判定用の全タイルの配列
    bool moveTo(Ivec2 _destination, std::vector<Tile> _tiles, const std::vector<Character>& _otherCharacters);
    void setCamera(SDL_Rect& _camera);
    void render(SDL_Rect& _camera);
    bool collided(std::vector<Tile> _tiles, Ivec2 _data_pos, std::vector<class Character> _otherCharacters);

    static LTexture mCharTexture;
    // LTexture mCharTexture;

    std::vector<SDL_Rect> sprile_clips;

    bool isMoved;

protected:
    /// @brief 矩形とタイルの衝突判定
    /// @param _tiles 全タイルの配列
    /// @param _data_pos 対象のデータ系座標
    bool touchWall(std::vector<Tile> _tiles, Ivec2 _data_pos);

    /// @brief キャラとキャラの衝突判定
    /// @param _otherCharacters 
    /// @param _data_pos 
    /// @return 当たっていたらtrue
    bool touchChars(const std::vector<class Character>& _otherCharacters, Ivec2 _data_pos);

    /// @brief キャラとキャラの衝突判定
    /// @param _otherCharacter 
    /// @param _data_pos 
    /// @return 当たっていたらtrue
    bool touchChar(const class Character & _otherCharacter, Ivec2 _data_pos);

    /// @brief マップ外か判定
    /// @return マップ外だったらtrue
    bool mapOver();

    int nowHP;
    int maxHP, STR, VIT;
    STATE mState;
    DIRECTION mDir;
    CHAR_TYPE mType;

    //プレイヤーの衝突判定
    SDL_Rect mBox;

    int mAnimFrame;
    bool receivingDamage;
    std::string mName;
};
