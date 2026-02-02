#pragma once
#include "common.h"
#include "TileSet.h"
#include "TileMap.h"
#include "IFieldEntity.h"
#include "FieldCharacter.h"
#include "Camera.h"
#include "CollisionChecker.h"

class Field {
private:
    TileSet m_tileSet;
    TileMap m_tileMap;

    Camera m_camera;
    CollisionChecker m_collisionChecker;

    std::vector<std::unique_ptr<FieldCharacter>> m_players;
    std::vector<std::unique_ptr<FieldCharacter>> m_enemies;

    int m_frameCount;
    int m_animationCounter;

public:
    Field();

    int getFrameId() const { return m_animationCounter; }

    // ローカル座標を絶対座標に変換
    std::pair<int, int> toAbsolute(int localX, int localY) const { return m_camera.toAbsolute(localX, localY); }

    const std::vector<std::unique_ptr<FieldCharacter>>& getPlayers() const { return m_players; }
    const std::vector<std::unique_ptr<FieldCharacter>>& getEnemies() const { return m_enemies; }

    void update(const MoveAmounts& amounts, const Direction& direction);

    // タイル1枚の幅を取得
    int getTileWidth()  const { return m_tileSet.getTileWidth();  }
    // タイル1枚の高さを取得
    int getTileHeight() const { return m_tileSet.getTileHeight(); }

    // ビューオフセット取得
    int getViewOffsetX() const { return m_camera.getViewOffsetX(); }
    int getViewOffsetY() const { return m_camera.getViewOffsetY(); }

    // ステージデータを読み込む
    bool load(const std::vector<std::string>& jsonFiles
            , const std::string& mapFile);

    // タイル番号取得
    int getTileImage(int num, int tileId) const;

    // タイルセット取得
    const TileSet& getTileSet() const { return m_tileSet; }
    // タイルマップ取得
    const TileMap& getTileMap() const { return m_tileMap; }
private:
    void updateAnimation();
};
