#include "Field.h"
#include "GameSettings.h"

Field::Field()
    : m_tileSet()
    , m_tileMap()
    , m_viewOffsetX(0)
    , m_viewOffsetY(0)
{
}

bool Field::isWall(Direction dir, int absCharaX, int absCharaY, int tileSizeX, int tileSizeY)
{
    const int spriteW = GameSettings::instance().getSpriteWidth();
    const int spriteH = GameSettings::instance().getSpriteHeight();

    int rightX = (absCharaX + spriteW / 4) / tileSizeX;
    int leftX  = (absCharaX + spriteW - spriteW / 4 - 1) / tileSizeX;

    int topY  = (absCharaY + tileSizeY + spriteH / 4) / tileSizeY;
    int btmY  = (absCharaY - 1) / tileSizeY + spriteH / tileSizeY;

    int screenTileCountY = m_tileMap.getTileHeightNum();
    int screenTileCountX = m_tileMap.getTileWidthNum();

    int lx, rx;

    switch (dir) {
    case Direction::Up:
        if (static_cast<int>((absCharaY + tileSizeY) / tileSizeY) <= 0) return true;
        return m_tileMap.get(rightX, topY) != 0
            || m_tileMap.get(leftX, topY)  != 0;

    case Direction::Down:
        if (btmY >= screenTileCountY) return true;
        return m_tileMap.get(rightX, btmY) != 0 
            || m_tileMap.get(leftX, btmY)  != 0;

    case Direction::Left: 
        if (static_cast<int>((absCharaX + tileSizeX + spriteW / 4) / tileSizeX) <= 0) return true;

        lx = (absCharaX + spriteW / 4) / tileSizeX;
        return m_tileMap.get(lx, topY) != 0
            || m_tileMap.get(lx, btmY) != 0;

    case Direction::Right:
        if (static_cast<int>((absCharaX - 1 + spriteW / 4) / tileSizeX) >= screenTileCountX - 1) return true;

        rx  = (absCharaX  + tileSizeX + spriteW / 4) / tileSizeX;
        return m_tileMap.get(rx, topY) != 0
            || m_tileMap.get(rx, btmY) != 0;

    return false;
    }
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
        , screenTileCountY + nextTopTileY <= m_tileMap.getTileHeightNum()
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
        , screenTileCountX + nextTopTileX <= m_tileMap.getTileWidthNum()
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

