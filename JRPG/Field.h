#pragma once
#include "common.h"
#include "TileSet.h"
#include "TileMap.h"

class Field {
private:
    TileSet m_tileSet;
    TileMap m_tileMap;

    int m_viewOffsetX;  // マップ内における画面左上のX座標（カメラ位置）
    int m_viewOffsetY;  // マップ内における画面左上のY座標（カメラ位置）

public:
    Field();

    // 指定方向に壁があるかどうかを判定
    bool isWall(Direction dir, int absCharaX, int absCharaY, int tileSizeX, int tileSizeY);
    // ローカル座標を絶対座標に変換
    std::pair<int, int> toAbsolute(int localX, int localY) const;

    MoveAmounts move(
          const MoveAmounts& amounts
        , int nextCharaX
        , int nextCharaY
        , int charaXMax
        , int charaYMax
    );

    int getViewOffsetX() const;
    int getViewOffsetY() const;

    // ステージデータを読み込む
    bool load(const std::vector<std::string>& jsonFiles
            , const std::string& mapFile);

    const TileSet& getTileSet() const { return m_tileSet; }
    const TileMap& getTileMap() const { return m_tileMap; }
};
