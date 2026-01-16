#include "DxLib.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "FieldCharacter.h"
#include "GameSettings.h"

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

    // 実際に動けるピクセル量
    int upMoveAmount = 0;
    int dwMoveAmount = 0;
    int ltMoveAmount = 0;
    int rtMoveAmount = 0;

    // 移動フラグ
    bool upFlag = false;
    bool dwFlag = false;
    bool ltFlag = false;
    bool rtFlag = false;
    Direction direction = Direction::None;

    // 絶対座標 ＝ キャラクタの座標 ＋ カメラ座標
    int absCharaX = m_character->getX() + m_field.getViewOffsetX();
    int absCharaY = m_character->getY() + m_field.getViewOffsetY();

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

    // 画面の中央（ピクセル）
    int screenCenterX = GameSettings::instance().getWindowWidth()  / 2;
    int screenCenterY = GameSettings::instance().getWindowHeight() / 2;

    // キャラクタの半分の大きさ（ピクセル）
    int charaHalfWidth  = m_character->getSpriteWidth() / 2;
    int charaHalfHeight = m_character->getSpriteHeight() / 2;

    // キャラクタが画面中央に来る時の絶対座標（マップ座標）
    int charaXMax = screenCenterX - charaHalfWidth  + m_field.getViewOffsetX();
    int charaYMax = screenCenterY - charaHalfHeight + m_field.getViewOffsetY(); 

    m_field.move(
          upMoveAmount, dwMoveAmount, ltMoveAmount, rtMoveAmount
        , absCharaX, absCharaY
        , charaXMax, charaYMax
    );

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
