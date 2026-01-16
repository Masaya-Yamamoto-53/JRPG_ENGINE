#include "DxLib.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "FieldCharacter.h"

FieldManager::FieldManager()
    : m_field()
    , m_character(nullptr)
{
}

void FieldManager::setCharacter(FieldCharacter* character) {
    m_character = character;
}

void FieldManager::update() {
    InputManager& inputManager = InputManager::instance();

    int durationS = inputManager.getKeyHoldFrames(KEY_INPUT_W);
    int durationW = inputManager.getKeyHoldFrames(KEY_INPUT_S);
    int durationA = inputManager.getKeyHoldFrames(KEY_INPUT_A);
    int durationD = inputManager.getKeyHoldFrames(KEY_INPUT_D);

    int upMoveAmount = 0;
    int dwMoveAmount = 0;
    int ltMoveAmount = 0;
    int rtMoveAmount = 0;

    bool upFlag = false;
    bool dwFlag = false;
    bool ltFlag = false;
    bool rtFlag = false;

    enum class Direction direction = Direction::None;

    // キー入力の調停処理
    if (durationS > durationW) {
        direction = Direction::Up;
        upMoveAmount = m_character->getMoveAmount();
        upFlag = true;
    }
    if (durationS < durationW) {
        direction = Direction::Down;
        dwMoveAmount = m_character->getMoveAmount();
        dwFlag = true;
    }
    if (durationA > durationD) {
        direction = Direction::Left;
        ltMoveAmount = m_character->getMoveAmount();
        ltFlag = true;
    }
    if (durationA < durationD) {
        direction = Direction::Right;
        rtMoveAmount = m_character->getMoveAmount();
        rtFlag = true;
    }

    if (m_character) {
        m_character->update(
              upMoveAmount, dwMoveAmount, ltMoveAmount, rtMoveAmount
            , upFlag, dwFlag, ltFlag, rtFlag
            , direction);
    }
}

void FieldManager::draw() {
    // フィールドの描画
    m_renderer.drawField(m_field);

    // キャラクターの描画
    if (m_character) {
        m_renderer.drawCharacter(*m_character);
    }
}

void FieldManager::load() {
    // タイルセット読み込み
    std::vector<std::string> tileFiles = {
          "assets\\feild\\00.bmp"
        , "assets\\feild\\01.bmp"
    };

    // マップ読み込み
    std::string mapFile = "assets\\map\\dungeon.csv";

    bool result = m_field.load(tileFiles, mapFile);
}
