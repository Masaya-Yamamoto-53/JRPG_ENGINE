#pragma once
#include "common.h"
#include "TileSet.h"
#include "TileMap.h"
#include "Camera.h"
#include "CollisionChecker.h"
#include "SimpleEnemyFactory.h"
#include "FieldCharacter.h"

#include "json.hpp"
using json = nlohmann::json;

class Field {
private:
    TileSet m_tileSet;
    TileMap m_tileMap;

    Camera m_camera;
    CollisionChecker m_collisionChecker;

    SimpleEnemyFactory m_factory;

    std::vector<std::unique_ptr<FieldCharacter>> m_players;
    std::vector<std::unique_ptr<FieldCharacter>> m_enemies;

public:
    Field();
    // 味方キャラクターを取得
    const std::vector<std::unique_ptr<FieldCharacter>>& getPlayers() const { return m_players; }
    // 敵キャラクターを取得
    const std::vector<std::unique_ptr<FieldCharacter>>& getEnemies() const { return m_enemies; }
    // 更新
    void update(const MoveAmounts& amounts, const Direction& direction);
    // ステージ全体を読み込む
    bool load(const std::string& path);
    // JSONファイルを読み込む
    bool loadJson(const std::string& path, json& out);
    // タイルセットを読み込む
    bool loadTilesets(const json& j);
    // マップを読み込む
    bool loadMap(const json& j);
    // 敵データを読み込む
    bool loadEnemies(const json& j);
    // カメラを取得
    const Camera& getCamera() const { return m_camera; }
    // タイルセット取得
    const TileSet& getTileSet() const { return m_tileSet; }
    // タイルマップ取得
    const TileMap& getTileMap() const { return m_tileMap; }
};
