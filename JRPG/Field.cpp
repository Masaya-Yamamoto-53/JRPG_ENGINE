#include "Field.h"
#include "GameSettings.h"

Field::Field()
    : m_tileSet()
    , m_tileMap()
    , m_viewOffsetX(0)
    , m_viewOffsetY(0)
{
}

void Field::moveDirection(
      int& moveCount      // キャラクタがこのフレームで移動しようとしている量
    , int& startPixel     // カメラの現在位置
    , int limitCondition  // カメラがこの方向に動けるか
    , int charaCondition  // キャラクタが画面中央付近を超えているか
    , int direction       // 移動方向（+1: 下/右、-1: 上/左）
    ) {
    int move = moveCount;

    // moveを最大値から1pxずつ減らしながら、
    // 「このフレームでカメラが動ける最大量」を探す
    while (move > 0) {
        if (limitCondition  // カメラがその方向に動けるか（マップ端チェック）
         && charaCondition  // キャラが画面中央より進んでいるか（スクロール条件）
            ) {
            startPixel += direction * move;
            moveCount -= move;
            break;
        }
        // 動けない場合は1px減らして再チェック
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
    int tileSizeY = GameSettings::instance().getFieldTileWidth();
    int tileSizeX = GameSettings::instance().getFieldTileHeight();

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

