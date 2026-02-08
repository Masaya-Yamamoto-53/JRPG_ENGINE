#include "SceneManager.h"
#include "FieldScene.h"

FieldScene::FieldScene(SceneManager* sceneManager)
    : m_sceneManager(sceneManager)
    , m_inputController()
    , m_field(sceneManager->getParty())
    , m_movementController()
{
    load();
}

void FieldScene::update() {
    // 入力継続時間を取得
    DirectionalHoldFrames holdFrames = m_inputController.getDirectionalHoldFrames();

    // キャラクタの移動可能量を取得
    MoveAmounts amounts = m_movementController.computeMoveAmounts(
        holdFrames, m_field.getPlayers()[0].get(), m_field);

    // 移動方向の決定(キャラクタの向きに使用)
    Direction direction = m_movementController.computeDirection(holdFrames, amounts);

    // キャラクタ更新
    m_field.update(amounts, direction);

    const auto& events = m_field.getEvents();

    for (const auto& event : events) {
        if (event.type == FieldEventType::Encounter) {
            // 戦闘シーンへ切り替え
            m_sceneManager->changeScene(SceneType::Battle);
        }
    }
    m_field.clearEnvets();
}

void FieldScene::render() {
    // フィールドの描画
    m_renderer.renderField(m_field);
    // キャラクターの描画
    m_renderer.renderCharacter(m_field, m_field.getPlayers(), m_field.getEnemies());
}

void FieldScene::load() {
    // ダンジョン情報
    std::string path = "assets\\dungeon.json";
    m_field.load(path);
}