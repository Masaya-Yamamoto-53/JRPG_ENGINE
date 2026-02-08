#include <fstream>
#include <memory>
#include <iomanip>
#include <sstream>
#include "Field.h"
#include "FieldCharacter.h"
#include "PlayerAnimationStrategy.h"
#include "EnemyAnimationStrategy.h"
#include "PlayerMovementStrategy.h"
#include "EnemyMovementStrategy.h"
#include "DebugManager.h"
#include "GameSettings.h"

Field::Field(const Party& party)
    : m_tileSet()
    , m_tileMap()
    , m_camera()
    , m_collisionChecker()
    , m_factory()
{
    loadPlayersFormParty(party);
}

static std::string toIdString(int id) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << id;
    return ss.str();
}

void Field::loadPlayersFormParty(const Party& party) {
    std::vector<int> ids = party.getMemberIds();
    if (ids.empty()) return;

    std::string idStr = toIdString(ids[0]);  // ← C++11対応ゼロ埋め

    auto playerCharacter = std::make_unique<FieldCharacter>(
        idStr,
        "assets\\characters\\players\\",
        std::make_unique<PlayerMovementStrategy>(),
        std::make_unique<PlayerAnimationStrategy>(),
        100, 100
    );

    m_players.push_back(std::move(playerCharacter));
}

void Field::update(const MoveAmounts& amounts, const Direction& direction) {
    if (m_players.empty()) return;

    // フィールド移動処理
    MoveAmounts playerAmounts = m_camera.applyScroll(
          amounts
        , m_players[0].get()->getX()
        , m_players[0].get()->getY()
        , m_players[0].get()->getSpriteWidth()
        , m_players[0].get()->getSpriteHeight()
        , m_tileSet.getTileWidth()
        , m_tileSet.getTileHeight()
        , m_tileMap.getTileWidthNum()
        , m_tileMap.getTileHeightNum()
    );

    // 味方キャラクタ移動処理
    for (auto& p : m_players) {
        p->update(playerAmounts, direction);
    }

    // 暫定
    MoveAmounts enemiesAmounts;
    enemiesAmounts.up    = amounts.up    - playerAmounts.up;
    enemiesAmounts.down  = amounts.down  - playerAmounts.down;
    enemiesAmounts.left  = amounts.left  - playerAmounts.left;
    enemiesAmounts.right = amounts.right - playerAmounts.right;

    // 敵キャラクタ移動処理
    for (auto& e : m_enemies) {
        e->update(enemiesAmounts, Direction::None);
    }

    // Update debug information
    DebugManager::instance().setCharacterPosition(
          m_players[0].get()->getX()
        , m_players[0].get()->getY()
        , m_players[0].get()->getSpriteWidth()
        , m_players[0].get()->getSpriteHeight()
    );

    // エンカウント判定
    for (auto& p : m_players) {
        for (auto& e : m_enemies) {
            if (m_collisionChecker.checkCharacterCollision(*p, *e)) {
                // エンカウントイベント追加
                FieldEvent event;
                event.type = FieldEventType::Encounter;
                //event.groupId = e->getGroup();
                m_events.push_back(event);
            }
        }
    }

    m_tileSet.incrementFrameCounter();
}

bool Field::load(const std::string& path) {
    json j;
    if (!loadJson(path, j)) return false;
    if (!loadTilesets(j))   return false;
    if (!loadMap(j))        return false;
    if (!loadEnemies(j))    return false;
    return true;
}

bool Field::loadJson(const std::string& path, json& out) {
    std::ifstream ifs(path);
    if (!ifs) {
        printf("Error: cannot open field json %s\n", path.c_str());
        return false;
    }

    try {
        ifs >> out;
    } catch (const std::exception& e) {
        printf("Error: JSON parse error in %s: %s\n", path.c_str(), e.what());
        return false;
    }

    return true;
}

bool Field::loadTilesets(const json& j) {
    if (!j.contains("tilesets") || !j["tilesets"].is_array()) {
        printf("Error: 'tilesets' is missing or not an array\n");
        return false;
    }

    std::vector<std::string> tilesetsPath;
    try {
        tilesetsPath = j["tilesets"].get<std::vector<std::string>>();
    } catch (const std::exception& e) {
        printf("Error: invalid tilesets format: %s\n", e.what());
        return false;
    }

    if (!m_tileSet.loadFromJson(tilesetsPath)) {
        printf("Error: tilesets load failed\n");
        return false;
    }

    return true;
}

bool Field::loadMap(const json& j) {
    if (!j.contains("map") || !j["map"].is_string()) {
        printf("Error: 'map' is missing or not a string\n");
        return false;
    }

    std::string mapFilePath = j["map"].get<std::string>();

    if (!m_tileMap.load(mapFilePath)) {
        printf("Error: TileMap load failed: %s\n", mapFilePath.c_str());
        return false;
    }

    return true;
}

bool Field::loadEnemies(const json& j) {
    if (!j.contains("enemies") || !j["enemies"].is_string()) {
        // 敵なしはエラーではない
        return true;
    }

    std::string enemyFilePath = j["enemies"].get<std::string>();
    std::ifstream ifs(enemyFilePath);
    if (!ifs) {
        printf("Error: cannot open enemies json %s\n", enemyFilePath.c_str());
        return false;
    }

    json j2;
    try {
        ifs >> j2;
    } catch (const std::exception& e) {
        printf("Error: JSON parse error in %s: %s\n", enemyFilePath.c_str(), e.what());
        return false;
    }

    if (!j2.contains("enemies") || !j2["enemies"].is_array()) {
        printf("Error: 'enemies' is missing or not an array in %s\n", enemyFilePath.c_str());
        return false;
    }

    for (auto& enemyData : j2["enemies"]) {
        if (!enemyData.contains("type") ||
            !enemyData.contains("x") ||
            !enemyData.contains("y")) {
            printf("Error: enemy data missing fields\n");
            continue;
        }

        std::string type = enemyData["type"];
        int x = enemyData["x"];
        int y = enemyData["y"];

        auto enemy = m_factory.create(type, x, y);
        if (!enemy) {
            printf("Error: Unknown enemy type: %s\n", type.c_str());
            continue;
        }

        m_enemies.push_back(std::move(enemy));
    }

    return true;
}
