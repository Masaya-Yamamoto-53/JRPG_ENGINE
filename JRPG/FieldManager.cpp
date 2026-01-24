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
{
}

void FieldManager::setCharacter(FieldCharacter* character) {
    m_character = character;
}

DurationInputs  FieldManager::computeDurationInputs() {
    InputManager& inputManager = InputManager::instance();

    DurationInputs durations;
    durations.up    = inputManager.getKeyHoldFrames(KEY_INPUT_W);
    durations.down  = inputManager.getKeyHoldFrames(KEY_INPUT_S);
    durations.left  = inputManager.getKeyHoldFrames(KEY_INPUT_A);
    durations.right = inputManager.getKeyHoldFrames(KEY_INPUT_D);

    return durations;
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

MoveAmounts FieldManager::computeAmounts(const DurationInputs& durations, int absCharaX, int absCharaY) {
    MoveAmounts amounts = { 0, 0, 0, 0, false, false, false, false };

    // キー入力の調停処理
    if (durations.up > durations.down) {
        amounts.up    = computeMoveAmount(absCharaX, absCharaY,  0, -1,
            [&](int x, int y, int w, int h) { return m_field.isWall(Direction::Up,    x, y, w, h); });
    }
    if (durations.up < durations.down) {
        amounts.down  = computeMoveAmount(absCharaX, absCharaY,  0, +1,
            [&](int x, int y, int w, int h) { return m_field.isWall(Direction::Down,  x, y, w, h); });
    }
    if (durations.left > durations.right) {
        amounts.left  = computeMoveAmount(absCharaX, absCharaY, -1,  0,
            [&](int x, int y, int w, int h) { return m_field.isWall(Direction::Left,  x, y, w, h); });
    }
    if (durations.left < durations.right) {
        amounts.right = computeMoveAmount(absCharaX, absCharaY,  1,  0,
            [&](int x, int y, int w, int h) { return m_field.isWall(Direction::Right, x, y, w, h); });
    }

    // 移動フラグ
    amounts.upFlag    = amounts.up > 0;
    amounts.downFlag  = amounts.down  > 0;
    amounts.leftFlag  = amounts.left > 0;
    amounts.rightFlag = amounts.right > 0;

    return amounts;
}

Direction FieldManager::computeDirection(const DurationInputs& durations) {
    Direction direction = Direction::None;


    direction = Direction::None;
    if (durations.up > durations.down) {
        direction = Direction::Up;
    }
    if (durations.up < durations.down) {
        direction = Direction::Down;
    }
    if (durations.left > durations.right) {
        direction = Direction::Left;
    }
    if (durations.left < durations.right) {
        direction = Direction::Right;
    }

    return direction;
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
    DurationInputs durations = computeDurationInputs();

    // 絶対座標 ＝ キャラクタの座標 ＋ カメラ座標
    auto absPos = m_field.toAbsolute(m_character->getX(), m_character->getY());
    int absCharaX = absPos.first;
    int absCharaY = absPos.second;

    // 移動可能量の計算
    MoveAmounts amounts = computeAmounts(durations, absCharaX, absCharaY);

    // 移動方向の決定(キャラクタの向きに使用)
    Direction direction = computeDirection(durations);

    // 画面の中央（ピクセル）
    int screenCenterX = GameSettings::instance().getWindowWidth()  / 2;
    int screenCenterY = GameSettings::instance().getWindowHeight() / 2;

    // キャラクタの半分の大きさ（ピクセル）
    int charaHalfWidth  = m_character->getSpriteWidth() / 2;
    int charaHalfHeight = m_character->getSpriteHeight() / 2;

    // キャラクタが画面中央に来る時の絶対座標（マップ座標）
    int charaXMax = screenCenterX - charaHalfWidth  + m_field.getViewOffsetX();
    int charaYMax = screenCenterY - charaHalfHeight + m_field.getViewOffsetY(); 

    // フィールド移動処理
    MoveAmounts charaAmounts = m_field.move(
          amounts
        , absCharaX, absCharaY
        , charaXMax, charaYMax
    );

    // キャラクタ更新
    if (m_character) {
        m_character->update(
              charaAmounts
            , direction);
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
