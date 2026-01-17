#pragma once
#include "TileSet.h"
#include "TileMap.h"

class Field {
private:
    TileSet m_tileSet;
    TileMap m_tileMap;

    int m_viewOffsetX;  // 開始位置
    int m_viewOffsetY;  // 開始位置
public:
    Field();

    bool isWallUp(int absCharaX, int absCharaY, int pixelXSize, int pixelYSize);
    bool isWallDw(int absCharaX, int absCharaY, int pixelXSize, int pixelYSize);
    bool isWallLt(int absCharaX, int absCharaY, int pixelXSize, int pixelYSize);
    bool isWallRt(int absCharaX, int absCharaY, int pixelXSize, int pixelYSize);

    void move(
          int& upMove
        , int& downMove
        , int& leftMove
        , int& rightMove
        , int nextCharaX
        , int nextCharaY
        , int charaXMax
        , int charaYMax
    );

    int getViewOffsetX() const;
    int getViewOffsetY() const;

    // ステージデータを読み込む
    bool load(const std::vector<std::string>& tileFiles
            , const std::string& mapFile);

    const TileSet& getTileSet() const { return m_tileSet; }
    const TileMap& getTileMap() const { return m_tileMap; }

private:
    void moveDirection(
          int& moveCount
        , int& startPixel
        , int limitCondition
        , int charaCondition
        , int step
    );
};
