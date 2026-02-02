#include <memory>
#include "Field.h"
#include "FieldCharacter.h"
#include "PlayerAnimationStrategy.h"
#include "EnemyAnimationStrategy.h"
#include "PlayerMovementStrategy.h"
#include "EnemyMovementStrategy.h"
#include "DebugManager.h"
#include "GameSettings.h"

Field::Field()
    : m_tileSet()
    , m_tileMap()
    , m_frameCount(0)
    , m_animationCounter(0)
    , m_camera()
{
    // 暫定対応
    auto playerCharacter = std::make_unique<FieldCharacter>(
          "00"
        , "assets\\characters\\players\\"
        , std::make_unique<PlayerMovementStrategy>()
        , std::make_unique<PlayerAnimationStrategy>()
    );

    auto enemiesCharacter = std::make_unique<FieldCharacter>(
          "goblin"
        , "assets\\characters\\enemies\\"
        , std::make_unique<EnemyMovementStrategy>(0, 100, 1)
        , std::make_unique<EnemyAnimationStrategy>()
    );

    m_players.push_back(std::move(playerCharacter));
    m_enemies.push_back(std::move(enemiesCharacter));
}

bool Field::isWall(
      Direction dir
    , int absCharaX, int absCharaY
    , int spriteW, int spriteH
) const
{
    // 当たり判定の左右端
    int hitLeft   = absCharaX + spriteW / 4;
    int hitRight  = absCharaX + spriteW - spriteW / 4 - 1;

    // 当たり判定の上下端
    int hitTop = absCharaY;                   // キャラクタの頭頂部
    int hitBottom = absCharaY + spriteH - 1;  // キャラクタの足元

    int tileSizeX = m_tileSet.getTileWidth();
    int tileSizeY = m_tileSet.getTileHeight();

    // タイル座標（衝突判定用）
    int leftX  = hitLeft / tileSizeX;
    int rightX = hitRight / tileSizeX;
    int topY = (absCharaY + spriteH - spriteH / 4) / tileSizeY;  // キャラクタの下半身
    int btmY   = hitBottom / tileSizeY;

    int mapTilesY = m_tileMap.getTileHeightNum();
    int mapTilesX = m_tileMap.getTileWidthNum();

    switch (dir) {
    case Direction::Up:
        // ピクセルベース境界チェック
        if (hitTop < 0) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, topY).first
                              , m_tileMap.get(rightX, topY).second, m_animationCounter)
            || m_tileSet.isWall(m_tileMap.get(leftX,  topY).first
                              , m_tileMap.get(leftX,  topY).second, m_animationCounter);

    case Direction::Down:
        // ピクセルベース境界チェック
        if (hitBottom >= mapTilesY * tileSizeY) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, btmY).first
                              , m_tileMap.get(rightX, btmY).second, m_animationCounter)
            || m_tileSet.isWall(m_tileMap.get(leftX,  btmY).first
                              , m_tileMap.get(leftX,  btmY).second, m_animationCounter);
    case Direction::Left: 
        // ピクセルベース境界チェック
        if (hitLeft < 0) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(leftX, topY).first
                              , m_tileMap.get(leftX, topY).second, m_animationCounter)
            || m_tileSet.isWall(m_tileMap.get(leftX, btmY).first
                              , m_tileMap.get(leftX, btmY).second, m_animationCounter);

    case Direction::Right:
        // ピクセルベース境界チェック
        if (hitRight >= mapTilesX * tileSizeX) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, topY).first
                              , m_tileMap.get(rightX, topY).second, m_animationCounter)
            || m_tileSet.isWall(m_tileMap.get(rightX, btmY).first
                              , m_tileMap.get(rightX, btmY).second, m_animationCounter);
    }
    return false;
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

void Field::updateAnimation() {
    m_frameCount++;
    if (m_frameCount % 30 == 0) {
        m_animationCounter++;
    }
}

bool Field::load(const std::vector<std::string>& jsonFiles
               , const std::string& mapFile
               ) {
    // タイルセット読み込み
    if (!m_tileSet.loadFromJson(jsonFiles)) {
        printf("Error: TileSet load failed\n");
        return false;
    }

    // マップ読み込み
    if (!m_tileMap.load(mapFile)) {
        printf("Error: TileMap load failed\n");
        return false;
    }

    return true;
}

int Field::getTileImage(int num, int tileId) const {
    return m_tileSet.getTileImage(num, tileId, m_animationCounter);
}
