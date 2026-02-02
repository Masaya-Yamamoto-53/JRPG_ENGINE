#include "DxLib.h"
#include "FieldManager.h"
#include "InputDeviceHub.h"
#include "FieldCharacter.h"

FieldManager::FieldManager()
    : m_inputController()
    , m_field()
    , m_movementController()
{
}

void FieldManager::update() {
    // 入力継続時間の取得
    DirectionalHoldFrames holdFrames = m_inputController.getDirectionalHoldFrames();

    // 移動可能量の計算
    MoveAmounts amounts = m_movementController.computeMoveAmounts(
        holdFrames, m_field.getPlayers()[0].get(), m_field);

    // 移動方向の決定(キャラクタの向きに使用)
    Direction direction = m_movementController.computeDirection(holdFrames, amounts);

    // キャラクタ更新
    m_field.update(amounts, direction);
}

void FieldManager::draw() {
    // フィールドの描画
    m_renderer.drawField(m_field);
    // キャラクターの描画
    m_renderer.drawCharacter(m_field, m_field.getPlayers(), m_field.getEnemies());
}

void FieldManager::load() {
    // タイルセット
    std::vector<std::string> jsonFiles = {
          "assets\\feild\\Tile00.json"
        , "assets\\feild\\Tile01.json"
        , "assets\\feild\\Tile02.json"
    };

    // マップファイル
    std::string mapFile = "assets\\map\\dungeon.csv";

    bool result = m_field.load(jsonFiles, mapFile);
}
