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
            || m_tileSet.isWall(left.first, left.second);

    case Direction::Down:
        if (btmY >= screenTileCountY) return true;
        right = m_tileMap.get(rightX, btmY);
        left  = m_tileMap.get(leftX, btmY);
        return m_tileSet.isWall(right.first, right.second)
            || m_tileSet.isWall(left.first, left.second);

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

// ローカル座標を絶対座標に変換
std::pair<int, int> Field::toAbsolute(int localX, int localY) const {
    return {
          localX + m_viewOffsetX
        , localY + m_viewOffsetY
    };
}

MoveAmounts Field::move(
      const MoveAmounts& amounts
    , int absCharaX, int absCharaY
    , int charaXMax, int charaYMax
    ) {

    // タイルのサイズ（1タイルのピクセル幅・高さ）
    int tileSizeY = GameSettings::instance().getFieldTileHeight();
    int tileSizeX = GameSettings::instance().getFieldTileWidth();

    // 下方向・右方向に動いた場合に、画面上端がどのタイルに位置するかを計算
    // （タイル境界を跨ぐかどうかの判定に使用）
    int nextTopTileY =
        ((m_viewOffsetY +  amounts.down + tileSizeY - 1) / tileSizeY);
    int nextTopTileX =
        ((m_viewOffsetX + amounts.right + tileSizeX - 1) / tileSizeX);

    // 画面に表示されるタイル数
    int screenTileCountY = GameSettings::instance().getTileCountY();
    int screenTileCountX = GameSettings::instance().getTileCountX();

    MoveAmounts results = amounts;

    // 下方向スクロール処理
    if ((screenTileCountY + nextTopTileY <= m_tileMap.getTileHeightNum())
     && (absCharaY >= charaYMax)) {
        m_viewOffsetY += amounts.down;
        results.down = 0;
    }

    // 上方向スクロール処理
    if ((m_viewOffsetY - amounts.up >= 0)
        && (absCharaY <= charaYMax)) {
        m_viewOffsetY -= amounts.up;
        results.up = 0;
    }

    // 左方向スクロール処理
    if ((m_viewOffsetX - amounts.left>= 0)
     && (absCharaX <= charaXMax)) {
        m_viewOffsetX -= amounts.left;
        results.left = 0;
    }

    // 右方向スクロール処理
    if ((screenTileCountX + nextTopTileX <= m_tileMap.getTileWidthNum())
     && (absCharaX >= charaXMax)) {
        m_viewOffsetX += amounts.right;
        results.right = 0;
    }

    return results;
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

