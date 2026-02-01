#pragma once
#include "common.h"
#include "TileSet.h"
#include "TileMap.h"
#include "IFieldEntity.h"
#include "FieldCharacter.h"

class Field {
private:
    TileSet m_tileSet;
    TileMap m_tileMap;

    int m_viewOffsetX;  // マップ内における画面左上のX座標（カメラ位置）
    int m_viewOffsetY;  // マップ内における画面左上のY座標（カメラ位置）

    std::vector<std::unique_ptr<FieldCharacter>> m_players;
    std::vector<std::unique_ptr<FieldCharacter>> m_enemies;

public:
    Field();

    // 指定方向に壁があるかどうかを判定
    bool isWall(
          Direction dir
        , int frameId
        , int absCharaX, int absCharaY
        , int tileSizeX, int tileSizeY
        , int spriteW, int spriteH
    ) const;
    // ローカル座標を絶対座標に変換
    std::pair<int, int> toAbsolute(int localX, int localY) const;
    // キャラクタの画面中央位置の絶対座標を計算
    std::pair<int, int> computeCharacterCenterAbsPos(const FieldCharacter* entity) const;

    const std::vector<std::unique_ptr<FieldCharacter>>& getPlayers() const { return m_players; }
    const std::vector<std::unique_ptr<FieldCharacter>>& getEnemies() const { return m_enemies; }

    void update(const MoveAmounts& amounts, const Direction& direction);

    // フィールド移動処理
    MoveAmounts applyScroll(
          const MoveAmounts& amounts
        //, const IFieldEntity& entity
    );

    // ビューオフセット取得
    int getViewOffsetX() const { return m_viewOffsetX; }
    int getViewOffsetY() const { return m_viewOffsetY; }

    // ステージデータを読み込む
    bool load(const std::vector<std::string>& jsonFiles
            , const std::string& mapFile);

    // タイルセット取得
    const TileSet& getTileSet() const { return m_tileSet; }
    // タイルマップ取得
    const TileMap& getTileMap() const { return m_tileMap; }

private:
    // スクロール可能かどうかを判定
    bool canScrollUp   (int viewOffsetY, int amountUp  ) const { return (viewOffsetY - amountUp  ) >= 0; }
    bool canScrollLeft (int viewOffsetX, int amountLeft) const { return (viewOffsetX - amountLeft) >= 0; }
    bool canScrollDown (int nextTopTileY, int screenTileCountY) const { return nextTopTileY + screenTileCountY <= m_tileMap.getTileHeightNum(); }
    bool canScrollRight(int nextTopTileX, int screenTileCountX) const { return nextTopTileX + screenTileCountX <= m_tileMap.getTileWidthNum();  }
    // キャラクタが画面中央を超えているかどうかを判定
    bool isCharacterAboveCenter  (int absCharaY, int charaYMax) const { return absCharaY <= charaYMax; }
    bool isCharacterBelowCenter  (int absCharaY, int charaYMax) const { return absCharaY >= charaYMax; }
    bool isCharacterLeftOfCenter (int absCharaX, int charaXMax) const { return absCharaX <= charaXMax; }
    bool isCharacterRightOfCenter(int absCharaX, int charaXMax) const { return absCharaX >= charaXMax; }

};
