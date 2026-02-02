#pragma once
#include <utility>
#include "common.h"

class Camera {
private:
    int m_viewOffsetX;  // マップ内における画面左上のX座標（カメラ位置）
    int m_viewOffsetY;  // マップ内における画面左上のY座標（カメラ位置）

public:
    Camera();
    MoveAmounts applyScroll(
          const MoveAmounts& amounts
        , int charaX, int charaY
        , int spriteW, int spriteH
        , int tileSizeX, int tileSizeY
        , int tileWidthNum, int tileHeightNum
    );

    int getViewOffsetX() const { return m_viewOffsetX; }
    int getViewOffsetY() const { return m_viewOffsetY; }
    // ローカル座標を絶対座標に変換
    std::pair<int,int> toAbsolute(int localX, int localY) const;

private:
    // 
    std::pair<int,int> computeCharacterCenterAbsPos(int spriteW, int spriteH) const;

    // スクロール可能かどうかを判定
    bool canScrollUp   (int viewOffsetY, int amountUp  ) const { return (viewOffsetY - amountUp  ) >= 0; }
    bool canScrollLeft (int viewOffsetX, int amountLeft) const { return (viewOffsetX - amountLeft) >= 0; }
    bool canScrollDown (int nextTopTileY, int screenTileCountY, int tileHeightNum) const { return nextTopTileY + screenTileCountY <= tileHeightNum; }
    bool canScrollRight(int nextTopTileX, int screenTileCountX, int tileWidthNum ) const { return nextTopTileX + screenTileCountX <= tileWidthNum;  }

    // キャラクタが画面中央を超えているかどうかを判定
    bool isCharacterAboveCenter  (int absCharaY, int charaYMax) const { return absCharaY <= charaYMax; }
    bool isCharacterBelowCenter  (int absCharaY, int charaYMax) const { return absCharaY >= charaYMax; }
    bool isCharacterLeftOfCenter (int absCharaX, int charaXMax) const { return absCharaX <= charaXMax; }
    bool isCharacterRightOfCenter(int absCharaX, int charaXMax) const { return absCharaX >= charaXMax; }
};
