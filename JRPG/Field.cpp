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

    // 当たり判定の左右端
    int hitLeft   = absCharaX + spriteW / 4;
    int hitRight  = absCharaX + spriteW - spriteW / 4 - 1;

    // 当たり判定の上下端
    int hitTop = absCharaY;                   // キャラクタの頭頂部
    int hitBottom = absCharaY + spriteH - 1;  // キャラクタの足元

    // タイル座標（衝突判定用）
    int leftX  = hitLeft / tileSizeX;
    int rightX = hitRight / tileSizeX;
    int topY = (absCharaY + spriteH - spriteH / 4) / tileSizeY;  // キャラクタの下半身
    int btmY   = hitBottom / tileSizeY;

    int mapTilesY = m_tileMap.getTileHeightNum();
    int mapTilesX = m_tileMap.getTileWidthNum();

    switch (dir) {
    case Direction::Up:
        // ピクセルベース境界チェック
        if (hitTop < 0) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, topY).first
                              , m_tileMap.get(rightX, topY).second)
            || m_tileSet.isWall(m_tileMap.get(leftX,  topY).first
                              , m_tileMap.get(leftX,  topY).second);

    case Direction::Down:
        // ピクセルベース境界チェック
        if (hitBottom >= mapTilesY * tileSizeY) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, btmY).first
                              , m_tileMap.get(rightX, btmY).second)
            || m_tileSet.isWall(m_tileMap.get(leftX,  btmY).first
                              , m_tileMap.get(leftX,  btmY).second);
    case Direction::Left: 
        // ピクセルベース境界チェック
        if (hitLeft < 0) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(leftX, topY).first
                              , m_tileMap.get(leftX, topY).second)
            || m_tileSet.isWall(m_tileMap.get(leftX, btmY).first
                              , m_tileMap.get(leftX, btmY).second);

    case Direction::Right:
        // ピクセルベース境界チェック
        if (hitRight >= mapTilesX * tileSizeX) return true;

        // タイル衝突判定
        return m_tileSet.isWall(m_tileMap.get(rightX, topY).first
                              , m_tileMap.get(rightX, topY).second)
            || m_tileSet.isWall(m_tileMap.get(rightX, btmY).first
                              , m_tileMap.get(rightX, btmY).second);
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
        // スクロール後の画面上端のY座標をタイルサイズで割る
        // (+ timeSizeY - 1は切り上げの為で、画面の端が切れないようにするため)
        (m_viewOffsetY + amounts.down  + tileSizeY - 1) / tileSizeY;
    int nextTopTileX =
        // スクロール後の画面上端のX座標をタイルサイズで割る
        // (+ timeSizeX - 1は切り上げの為で、画面の端が切れないようにするため)
        (m_viewOffsetX + amounts.right + tileSizeX - 1) / tileSizeX;

    // 画面に表示されるタイル数
    int screenTileCountY = GameSettings::instance().getTileCountY();
    int screenTileCountX = GameSettings::instance().getTileCountX();

    MoveAmounts results = amounts;

    // 下方向スクロール処理
    //    画面下端がマップの下端を超えていない 
    // && キャラクタが画面中央を超えている
    if ((screenTileCountY + nextTopTileY <= m_tileMap.getTileHeightNum())
     && (absCharaY >= charaYMax)) {
        m_viewOffsetY += amounts.down;
        results.down = 0;
    }

    // 右方向スクロール処理
    //    画面右端がマップの右端を超えていない
    // && キャラクタが画面中央を超えている
    if ((screenTileCountX + nextTopTileX <= m_tileMap.getTileWidthNum())
     && (absCharaX >= charaXMax)) {
        m_viewOffsetX += amounts.right;
        results.right = 0;
    }

    // 上方向スクロール処理
    //    画面上端がマップの上端を超えていない
    // && キャラクタが画面中央を超えている
    if ((m_viewOffsetY - amounts.up   >= 0) && (absCharaY <= charaYMax)) {
        m_viewOffsetY -= amounts.up;
        results.up = 0;
    }

    // 左方向スクロール処理
    //   画面左端がマップの左端を超えていない
    // && キャラクタが画面中央を超えている
    if ((m_viewOffsetX - amounts.left >= 0) && (absCharaX <= charaXMax)) {
        m_viewOffsetX -= amounts.left;
        results.left = 0;
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

