#include "Camera.h"
#include "GameSettings.h"

Camera::Camera()
    : m_viewOffsetX(0)
    , m_viewOffsetY(0)
{
}

MoveAmounts Camera::applyScroll(
      const MoveAmounts& amounts
    , int charaX, int charaY
    , int spriteW, int spriteH
    , int tileSizeX, int tileSizeY
    , int tileWidthNum, int tileHeightNum
) {
    // 絶対座標 ＝ キャラクタの座標 ＋ カメラ座標
    auto charaAbsPos = toAbsolute(charaX, charaY);
    int charaAbsX = charaAbsPos.first;
    int charaAbsY = charaAbsPos.second;

    // キャラクタが画面中央に来る時の絶対座標（マップ座標）
    auto charaCenterAbsPos = computeCharacterCenterAbsPos(spriteW, spriteH);
    int charaCenterAbsX = charaCenterAbsPos.first;
    int charaCenterAbsY = charaCenterAbsPos.second;

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

    // 画面に表示できるタイル数
    int screenTileCountY = GameSettings::instance().getTileCountY();
    int screenTileCountX = GameSettings::instance().getTileCountX();

    MoveAmounts results = amounts;

    // 上方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollUp(m_viewOffsetY, amounts.up) && isCharacterAboveCenter(charaAbsY, charaCenterAbsY)) {
        m_viewOffsetY -= amounts.up;
        results.up = 0;
    }

    // 下方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollDown(nextTopTileY, screenTileCountY, tileHeightNum) && isCharacterBelowCenter(charaAbsY, charaCenterAbsY)) {
        m_viewOffsetY += amounts.down;
        results.down = 0;
    }

    // 左方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollLeft(m_viewOffsetX, amounts.left) && isCharacterLeftOfCenter(charaAbsX, charaCenterAbsX)) {
        m_viewOffsetX -= amounts.left;
        results.left = 0;
    }

    // 右方向スクロール処理
    // スクロール可能 && キャラクタが画面中央を超えている
    if (canScrollRight(nextTopTileX, screenTileCountX, tileWidthNum) && isCharacterRightOfCenter(charaAbsX, charaCenterAbsX)) {
        m_viewOffsetX += amounts.right;
        results.right = 0;
    }

    return results;
}

std::pair<int,int> Camera::toAbsolute(int localX, int localY) const {
    return {
          localX + m_viewOffsetX
        , localY + m_viewOffsetY
    };
}

std::pair<int,int> Camera::computeCharacterCenterAbsPos(int spriteW, int spriteH) const {
    int screenCenterX = GameSettings::instance().getWindowWidth()  / 2;
    int screenCenterY = GameSettings::instance().getWindowHeight() / 2;

    int charaHalfWidth  = spriteW / 2;
    int charaHalfHeight = spriteH / 2;

    return {
        screenCenterX - charaHalfWidth  + m_viewOffsetX,
        screenCenterY - charaHalfHeight + m_viewOffsetY
    };
}

