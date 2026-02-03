#include <fstream>
#include <memory>
#include "Field.h"
#include "FieldCharacter.h"
#include "PlayerAnimationStrategy.h"
#include "EnemyAnimationStrategy.h"
#include "PlayerMovementStrategy.h"
#include "EnemyMovementStrategy.h"
#include "DebugManager.h"
#include "GameSettings.h"

#include "json.hpp"
using json = nlohmann::json;

Field::Field()
    : m_tileSet()
    , m_tileMap()
    , m_frameCount(0)
    , m_animationCounter(0)
    , m_camera()
    , m_collisionChecker()
    , m_factory()
{
    // 暫定対応
    auto playerCharacter = std::make_unique<FieldCharacter>(
          "00"
        , "assets\\characters\\players\\"
        , std::make_unique<PlayerMovementStrategy>()
        , std::make_unique<PlayerAnimationStrategy>()
        , 100, 100
    );

    //auto enemiesCharacter = std::make_unique<FieldCharacter>(
    //      "goblin"
    //    , "assets\\characters\\enemies\\"
    //    , std::make_unique<EnemyMovementStrategy>(0, 100, 1)
    //    , std::make_unique<EnemyAnimationStrategy>()
    //    , 150, 150
    //);

    m_players.push_back(std::move(playerCharacter));
    //m_enemies.push_back(std::move(enemiesCharacter));
}

void Field::update(const MoveAmounts& amounts, const Direction& direction) {
    // フィールド移動処理
    //MoveAmounts playerAmounts = applyScroll(amounts);

    MoveAmounts playerAmounts = m_camera.applyScroll(amounts
        , m_players[0].get()->getX(), m_players[0].get()->getY()
        , m_players[0].get()->getSpriteWidth(), m_players[0].get()->getSpriteHeight()
        , m_tileSet.getTileWidth(), m_tileSet.getTileHeight()
        , m_tileMap.getTileWidthNum(), m_tileMap.getTileHeightNum()
    );

    // 味方キャラクタ移動処理
    for (auto& p : m_players) {
        p->update(playerAmounts, direction);
    }

    MoveAmounts enemiesAmounts;
    enemiesAmounts.up    = amounts.up    - playerAmounts.up;
    enemiesAmounts.down  = amounts.down  - playerAmounts.down;
    enemiesAmounts.left  = amounts.left  - playerAmounts.left;
    enemiesAmounts.right = amounts.right - playerAmounts.right;

    // 敵キャラクタ移動処理
    for (auto& e : m_enemies) {
        e->update(enemiesAmounts, direction);
    }

    // Update debug information
    DebugManager::instance().setCharacterPosition(
          m_players[0].get()->getX()
        , m_players[0].get()->getY()
        , m_players[0].get()->getSpriteWidth()
        , m_players[0].get()->getSpriteHeight()
    );

    // アニメーションカウンタ更新
    updateAnimation();
}

bool Field::load(const std::string& path) {
    std::ifstream ifs(path);
    json j;
    ifs >> j;

    // タイルセット読み込み
    std::vector<std::string> tilesetsPath;
    if (j.contains("tilesets")) {
        tilesetsPath = j["tilesets"].get<std::vector<std::string>>();
    }
    if (!m_tileSet.loadFromJson(tilesetsPath)) {
        printf("Error: TileSet load failed\n");
        return false;
    }

    // マップ読み込み
    std::string mapFilePath;
    if (j.contains("map")) {
        mapFilePath = j["map"].get<std::string>();
    }
    if (!m_tileMap.load(mapFilePath)) {
        printf("Error: TileMap load failed\n");
        return false;
    }

    // 敵を生成
    std::string enemyFilePath;
    if (j.contains("enemies")) {
        enemyFilePath = j["enemies"].get<std::string>();
    }
    std::ifstream ifs2(enemyFilePath);
    json j2;
    if (!ifs2) {
        printf("JSON error: cannot open %s\n", enemyFilePath.c_str());
    }
    ifs2 >> j2;

    if (j2.contains("enemies")) {
        printf("Hit\n");
        for (auto& enemyData : j2["enemies"]) {

            // 敵タイプ
            std::string type = enemyData["type"];

            // spawn情報
            auto& spawn = enemyData["spawn"];
            int x = enemyData["x"];
            int y = enemyData["y"];

            // Factoryで生成
            auto enemy = m_factory.create(type, x, y);
            if (enemy) {
                m_enemies.push_back(std::move(enemy));
            } else {
                printf("Error: Unknown enemy type: %s\n", type.c_str());
            }
        }
    }

    return true;
}

int Field::getTileImage(int num, int tileId) const {
    return m_tileSet.getTileImage(num, tileId, m_animationCounter);
}

void Field::updateAnimation() {
    m_frameCount++;
    if (m_frameCount % 30 == 0) {
        m_animationCounter++;
    }
}

