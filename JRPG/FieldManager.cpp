#include "DxLib.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "FieldCharacter.h"
#include "GameSettings.h"

FieldManager::FieldManager()
    : m_field()
    , m_character(nullptr)
    , m_frameCount(0)
    , m_animationCounter(0)
    , m_movementController()
{
}

void FieldManager::setCharacter(FieldCharacter* character) {
    m_character = character;
}

int FieldManager::computeMoveAmount(int baseX, int baseY, int deltaX, int deltaY,
    std::function<bool(int, int, int, int)> isWallFunc) {
    int maxMove = m_character->getMoveAmount();
    int pixelXSize = GameSettings::instance().getFieldTileWidth();
    int pixelYSize = GameSettings::instance().getFieldTileHeight();

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
    if (m_frameCount % 20 == 0) {
        m_animationCounter++;
    }
}

void FieldManager::update() {
    // 入力継続時間の取得
    DirectionalHoldFrames holdFrames = InputManager::instance().getDirectionalHoldFrames();

    // 移動可能量の計算
    MoveAmounts amounts = m_movementController.computeMoveAmounts(holdFrames ,*m_character ,m_field );

    // 移動方向の決定(キャラクタの向きに使用)
    Direction direction = m_movementController.computeDirection(holdFrames, amounts);

    // フィールド移動処理
    MoveAmounts charaAmounts = m_field.applyScroll(amounts , *m_character);

    // キャラクタ更新
    if (m_character) {
        m_character->update(charaAmounts, direction);
    }

    // アニメーション更新
    updateAnimation();
}

void FieldManager::draw() {
    // フィールドの描画
    m_renderer.drawField(m_field, m_animationCounter);

    // キャラクターの描画
    if (m_character) {
        m_renderer.drawCharacter(*m_character);
    }
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
