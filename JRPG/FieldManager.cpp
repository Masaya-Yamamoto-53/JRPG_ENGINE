#include "DxLib.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "FieldCharacter.h"

FieldManager::FieldManager()
    : m_field()
    , m_frameCount(0)
    , m_animationCounter(0)
    , m_movementController()
{
}

int FieldManager::computeMoveAmount(int baseX, int baseY, int deltaX, int deltaY,
    std::function<bool(int, int, int, int)> isWallFunc) {
    int maxMove = m_field.getPlayers()[0]->getMoveAmount();
    int pixelXSize = m_field.getTileSet().getTileWidth();
    int pixelYSize = m_field.getTileSet().getTileHeight();

    while (maxMove > 0) {
        int nextX = baseX + deltaX * maxMove;
        int nextY = baseY + deltaY * maxMove;
        if (!isWallFunc(nextX, nextY, pixelXSize, pixelYSize)) {
            return maxMove;  // 移動可能量を返す
        }
        maxMove--;
    }
    return 0;
}

void FieldManager::updateAnimation() {
    // アニメーションカウンタ更新
    m_frameCount++;
    if (m_frameCount % 30 == 0) {
        m_animationCounter++;
    }
}

void FieldManager::update() {
    // 入力継続時間の取得
    DirectionalHoldFrames holdFrames = InputManager::instance().getDirectionalHoldFrames();

    // 移動可能量の計算
    MoveAmounts amounts = m_movementController.computeMoveAmounts(
        holdFrames , m_animationCounter, m_field.getPlayers()[0].get(), m_field );

    // 移動方向の決定(キャラクタの向きに使用)
    Direction direction = m_movementController.computeDirection(holdFrames, amounts);

    // キャラクタ更新
    m_field.update(amounts, direction);
}

void FieldManager::draw() {
    // フィールドの描画
    m_renderer.drawField(m_field, m_animationCounter);
    // キャラクターの描画
    m_renderer.drawCharacter(m_field, m_field.getPlayers(), m_field.getEnemies());
    // アニメーション更新
    updateAnimation();
}

void FieldManager::load() {
    // タイルセット読み込み
    std::vector<std::string> jsonFiles = {
          "assets\\feild\\Tile00.json"
        , "assets\\feild\\Tile01.json"
        , "assets\\feild\\Tile02.json"
    };

    // マップ読み込み
    std::string mapFile = "assets\\map\\dungeon.csv";

    bool result = m_field.load(jsonFiles, mapFile);
}
