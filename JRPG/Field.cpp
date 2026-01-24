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
    std::pair<int, int> right, left, top, btm;

    switch (dir) {
    case Direction::Up:
        if (static_cast<int>((absCharaY + tileSizeY) / tileSizeY) <= 0) return true;
        right = m_tileMap.get(rightX, topY);
        left  = m_tileMap.get(leftX, topY);
        return m_tileSet.isWall(right.first, right.second)
            || m_tileSet.isWall(left.first, right.second);

    case Direction::Down:
        if (btmY >= screenTileCountY) return true;
        right = m_tileMap.get(rightX, topY);
        left  = m_tileMap.get(leftX, topY);
        return m_tileSet.isWall(right.first, right.second)
            || m_tileSet.isWall(left.first, right.second);

    case Direction::Left: 
        if (static_cast<int>((absCharaX + tileSizeX + spriteW / 4) / tileSizeX) <= 0) return true;

        lx = (absCharaX + spriteW / 4) / tileSizeX;
        top = m_tileMap.get(lx, topY);
        btm = m_tileMap.get(lx, btmY);
        return m_tileSet.isWall(top.first, top.second)
            || m_tileSet.isWall(btm.first, btm.second);

    case Direction::Right:
        if (static_cast<int>((absCharaX - 1 + spriteW / 4) / tileSizeX) >= screenTileCountX - 1) return true;

        rx  = (absCharaX  + tileSizeX + spriteW / 4) / tileSizeX;
        top = m_tileMap.get(rx, topY);
        btm = m_tileMap.get(rx, btmY);
        return m_tileSet.isWall(top.first, top.second)
            || m_tileSet.isWall(btm.first, btm.second);
    }
    return false;
}

std::pair<int, int> Field::toAbsolute(int localX, int localY) const {
    return {
          localX + m_viewOffsetX
        , localY + m_viewOffsetY
    };
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

