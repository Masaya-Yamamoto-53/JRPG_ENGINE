#include "Field.h"
#include "GameSettings.h"

Field::Field()
    : m_tileSet()
    , m_tileMap()
    , m_viewOffsetX(0)
    , m_viewOffsetY(0)
{
}

bool Field::isWallUp(int absCharaX, int absCharaY, int tileSizeX, int tileSizeY) {
    int topRightX = (absCharaX + GameSettings::instance().getSpriteWidth() / 4) / tileSizeX;
    int topLeftX  = (absCharaX + GameSettings::instance().getSpriteWidth() - GameSettings::instance().getSpriteWidth() / 4 - 1) / tileSizeX;
    int topY      = (absCharaY + tileSizeY + GameSettings::instance().getFieldTileHeight() / 2 - 1) / tileSizeY;

    if (((absCharaY + tileSizeY) / tileSizeY) <= 0) return true;

    return m_tileMap.get(topRightX, topY) != 0   // 左端
        || m_tileMap.get(topLeftX, topY)  != 0;  // 右端
}

bool Field::isWallDw(int absCharaX, int absCharaY, int tileSizeX, int tileSizeY) {
    int btmRightX = (absCharaX + GameSettings::instance().getSpriteWidth() / 4) / tileSizeX;
    int btmLeftX  = (absCharaX + GameSettings::instance().getSpriteWidth() - GameSettings::instance().getSpriteWidth() / 4 - 1) / tileSizeX;

                    // キャラクタの上端（absCharaY）が属するタイルYを求める
                    // -1 しているのは、ちょうどタイル境界に乗ったときの off-by-one を防ぐため
    int btmY      = (absCharaY - 1) / tileSizeY
                    // キャラクタのスプライトの高さを「タイル何個分か」に変換して加算する
                    // これにより、キャラの下端（足元）が属するタイルYが求まる
                  + GameSettings::instance().getSpriteHeight() / tileSizeY;

    int screenTileCountY = m_tileMap.getTileHeightNum();

    // キャラの足元（btmY）が、画面に表示されるタイル数（screenTileCountY）
    // を超えてしまった場合は、画面外＋マップ外に出ていると判断して壁扱いにする。
    if (btmY >= screenTileCountY) return true;

    return m_tileMap.get(btmRightX, btmY) != 0   // 左端
        || m_tileMap.get(btmLeftX, btmY)  != 0;  // 右端
}

bool Field::isWallLt(int absCharaX, int absCharaY, int tileSizeX, int tileSizeY) {
    int leftTopY  = (absCharaY + tileSizeY + GameSettings::instance().getFieldTileHeight() / 2 - 1) / tileSizeY;
    int leftBtmY  = (absCharaY - 1) / tileSizeY
                  + GameSettings::instance().getSpriteHeight() / tileSizeY;

    // キャラクタの胴体付近に調整する
    // + GameSettings::instance().getSpriteWidth() / 4
    int leftX     = (absCharaX + tileSizeX + GameSettings::instance().getSpriteWidth() / 4) / tileSizeX;

    // キャラの体（leftX）が、画面に表示されるタイル数（0）
    // を超えてしまった場合は、画面外＋マップの外い出ていると判断して壁扱いにする。
    if (leftX <= 0) return true;

    leftX     = (absCharaX + GameSettings::instance().getSpriteWidth() / 4) / tileSizeX;
    return m_tileMap.get(leftX, leftTopY) != 0
        || m_tileMap.get(leftX, leftBtmY) != 0;
}

bool Field::isWallRt(int absCharaX, int absCharaY, int tileSizeX, int tileSizeY) {
    int rightX    = (absCharaX - 1 + GameSettings::instance().getSpriteWidth() / 4) / tileSizeX;
    int rightTopY = (absCharaY + tileSizeY + GameSettings::instance().getFieldTileHeight() / 2 - 1) / tileSizeY;
    int rightBtmY = (absCharaY - 1) / tileSizeY
                  + GameSettings::instance().getSpriteHeight() / tileSizeY;
    int screenTileCountX = m_tileMap.getTileWidthNum();

    if (rightX >= screenTileCountX - 1) return true;

    rightX    = (absCharaX - 1 + tileSizeX + GameSettings::instance().getSpriteWidth() / 4) / tileSizeX;
    return m_tileMap.get(rightX, rightTopY) != 0
        || m_tileMap.get(rightX, rightBtmY) != 0;
}

void Field::moveDirection(
      int& moveCount
    , int& startPixel
    , int limitCondition
    , int charaCondition
    , int direction
    ) {
    int move = moveCount;
    while (move > 0) {
        if (limitCondition && charaCondition) {
            startPixel += direction * move;
            moveCount -= move;
            break;
        }
        move--;
    }
}

void Field::move(
      int& upMoveAmount
    , int& downMoveAmount
    , int& leftMoveAmount
    , int& rightMoveAmount
    , int absCharaX, int absCharaY
    , int charaXMax, int charaYMax
    ) {

    // タイルのサイズ（1タイルのピクセル幅・高さ）
    int tileSizeY = GameSettings::instance().getFieldTileHeight();
    int tileSizeX = GameSettings::instance().getFieldTileWidth();

    // 下方向・右方向に動いた場合に、画面上端がどのタイルに位置するかを計算
    // （タイル教会を跨ぐかどうかの判定に使用）
    int nextTopTileY =
        ((m_viewOffsetY +  downMoveAmount + tileSizeY - 1) / tileSizeY);
    int nextTopTileX =
        ((m_viewOffsetX + rightMoveAmount + tileSizeX - 1) / tileSizeX);

    // 画面に表示されるタイル数
    int screenTileCountY = GameSettings::instance().getTileCountY();
    int screenTileCountX = GameSettings::instance().getTileCountX();

    // 下方向スクロール処理
    moveDirection(
          downMoveAmount
        , m_viewOffsetY
        , screenTileCountY + nextTopTileY <= m_tileMap.getTileWidthNum()
        , absCharaY >= charaYMax
        , +1
    );

    // 上方向スクロール処理
    moveDirection(
          upMoveAmount
        , m_viewOffsetY
        , m_viewOffsetY - upMoveAmount >= 0
        , absCharaY <= charaYMax
        , -1
    );

    // 左方向スクロール処理
    moveDirection(
          leftMoveAmount
        , m_viewOffsetX
        , m_viewOffsetX - leftMoveAmount >= 0
        , absCharaX <= charaXMax
        , -1
    );

    // 右方向スクロール処理
    moveDirection(
          rightMoveAmount
        , m_viewOffsetX
        , screenTileCountX + nextTopTileX <= m_tileMap.getTileHeightNum()
        , absCharaX >= charaXMax
        , +1
    );
}

int Field::getViewOffsetX() const { return m_viewOffsetX; }
int Field::getViewOffsetY() const { return m_viewOffsetY; }

bool Field::load(const std::vector<std::string>& tileFiles
               , const std::string& mapFile
               ) {
    // タイルセット読み込み
    if (!m_tileSet.load(tileFiles)) {
        return false;
    }

    // マップ読み込み
    if (!m_tileMap.load(mapFile)) {
        return false;
    }

    return true;
}

