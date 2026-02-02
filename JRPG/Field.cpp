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
    , m_viewOffsetX(0)
    , m_viewOffsetY(0)
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
    , int frameId
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
                              , m_tileMap.get(rightX, topY).second, frameId)
            || m_tileSet.isWall(m_tileMap.get(leftX,  topY).first
                              , m_tileMap.get(leftX,  topY).second, frameId);

    case Direction::Down:
        // ピクセルベース境界チェック
        if (hitBottom >= mapTilesY * tileSizeY) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, btmY).first
                              , m_tileMap.get(rightX, btmY).second, frameId)
            || m_tileSet.isWall(m_tileMap.get(leftX,  btmY).first
                              , m_tileMap.get(leftX,  btmY).second, frameId);
    case Direction::Left: 
        // ピクセルベース境界チェック
        if (hitLeft < 0) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(leftX, topY).first
                              , m_tileMap.get(leftX, topY).second, frameId)
            || m_tileSet.isWall(m_tileMap.get(leftX, btmY).first
                              , m_tileMap.get(leftX, btmY).second, frameId);

    case Direction::Right:
        // ピクセルベース境界チェック
        if (hitRight >= mapTilesX * tileSizeX) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, topY).first
                              , m_tileMap.get(rightX, topY).second, frameId)
            || m_tileSet.isWall(m_tileMap.get(rightX, btmY).first
                              , m_tileMap.get(rightX, btmY).second, frameId);
    }
    return false;
}

// ローカル座標を絶対座標に変換
std::pair<int, int> Field::toAbsolute(int localX, int localY) const {
    return {
          localX + m_viewOffsetX
        , localY + m_viewOffsetY
    };
}

std::pair<int,int> Field::computeCharacterCenterAbsPos(const FieldCharacter* entity) const {
    int screenCenterX = GameSettings::instance().getWindowWidth()  / 2;
    int screenCenterY = GameSettings::instance().getWindowHeight() / 2;

    int charaHalfWidth  = entity->getSpriteWidth()  / 2;
    int charaHalfHeight = entity->getSpriteHeight() / 2;

    return {
        screenCenterX - charaHalfWidth  + m_viewOffsetX,
        screenCenterY - charaHalfHeight + m_viewOffsetY
    };
}

MoveAmounts Field::applyScroll(const MoveAmounts& amounts) {

    FieldCharacter* chara = m_players[0].get();

    // 絶対座標 ＝ キャラクタの座標 ＋ カメラ座標
    auto charaAbsPos = toAbsolute(chara->getX(), chara->getY());
    int charaAbsX = charaAbsPos.first;
    int charaAbsY = charaAbsPos.second;

    // キャラクタが画面中央に来る時の絶対座標（マップ座標）
    auto charaCenterAbsPos = computeCharacterCenterAbsPos(chara);
    int charaCenterAbsX = charaCenterAbsPos.first;
    int charaCenterAbsY = charaCenterAbsPos.second;

    // タイルのサイズ（1タイルのピクセル幅・高さ）
    int tileSizeY = m_tileSet.getTileHeight();
    int tileSizeX = m_tileSet.getTileWidth();

    // 下方向・右方向に動いた場合に、画面上端がどのタイルに位置するかを計算
    // （タイル境界を跨ぐかどうかの判定に使用）
    int nextTopTileY =
        // スクロール後の画面上端のY座標をタイルサイズで割る
        // (+ timeSizeY - 1は切り上げの為で、画面の端が切れないようにするため)
        (m_viewOffsetY + amounts.down  + tileSizeY - 1) / tileSizeY;
    int nextTopTileX =
        // スクロール後の画面上端のX座標をタイルサイズで割る
        // (+ timeSizeX - 1は切り上げの為で、画面の端が切れないようにするため)
        (m_viewOffsetX + amounts.right + tileSizeX - 1) / tileSizeX;

    // 画面に表示できるタイル数
    int screenTileCountY = GameSettings::instance().getTileCountY();
    int screenTileCountX = GameSettings::instance().getTileCountX();

    MoveAmounts results = amounts;

    // 上方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollUp(m_viewOffsetY, amounts.up) && isCharacterAboveCenter(charaAbsY, charaCenterAbsY)) {
        m_viewOffsetY -= amounts.up;
        results.up = 0;
    }

    // 下方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollDown(nextTopTileY, screenTileCountY) && isCharacterBelowCenter(charaAbsY, charaCenterAbsY)) {
        m_viewOffsetY += amounts.down;
        results.down = 0;
    }

    // 左方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollLeft(m_viewOffsetX, amounts.left) && isCharacterLeftOfCenter(charaAbsX, charaCenterAbsX)) {
        m_viewOffsetX -= amounts.left;
        results.left = 0;
    }

    // 右方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollRight(nextTopTileX, screenTileCountX) && isCharacterRightOfCenter(charaAbsX, charaCenterAbsX)) {
        m_viewOffsetX += amounts.right;
        results.right = 0;
    }

    return results;
}

void Field::update(const MoveAmounts& amounts, const Direction& direction) {
    // フィールド移動処理
    MoveAmounts playerAmounts = applyScroll(amounts);

    // 味方キャラクタ更新
    m_players[0].get()->setMoveAmounts(playerAmounts); // 味方キャラクターが移動する距離
    m_players[0].get()->setDirection(direction);
    m_players[0].get()->update();

    // Update debug information
    DebugManager::instance().setCharacterPosition(
          m_players[0].get()->getX()
        , m_players[0].get()->getY()
        , m_players[0].get()->getSpriteWidth()
        , m_players[0].get()->getSpriteHeight()
    );

    MoveAmounts enemiesAmounts;
    enemiesAmounts.up    = amounts.up    - playerAmounts.up;
    enemiesAmounts.down  = amounts.down  - playerAmounts.down;
    enemiesAmounts.left  = amounts.left  - playerAmounts.left;
    enemiesAmounts.right = amounts.right - playerAmounts.right;

    // 敵キャラクタ更新
    for (auto& e : m_enemies) {
        e->setMoveAmounts(enemiesAmounts);
        e->update();
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

