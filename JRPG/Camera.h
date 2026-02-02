#pragma once
#include <utility>
#include "common.h"

class Camera {
private:
    int m_viewOffsetX;  // マップ内における画面左上のX座標（カメラ位置）
    int m_viewOffsetY;  // マップ内における画面左上のY座標（カメラ位置）

public:
    Camera();
    // キャラクタの移動量と位置から、必要に応じてスクロールを行う
    // スクロールした分だけMoveAmoutsを調整して返す
    MoveAmounts applyScroll(
          const MoveAmounts& amounts           // キャラクタの移動量
        , int charaX, int charaY               // キャラクタのローカル座標
        , int spriteW, int spriteH             // キャラクタのスプライトサイズ
        , int tileSizeX, int tileSizeY         // タイル1枚のピクセルサイズ
        , int tileWidthNum, int tileHeightNum  // マップの横/縦タイル数
    );

    // 現在のビューオフセットを取得
    int getViewOffsetX() const { return m_viewOffsetX; }
    int getViewOffsetY() const { return m_viewOffsetY; }

    // ローカル座標を絶対座標に変換
    std::pair<int,int> toAbsolute(int localX, int localY) const;

private:
    // キャラクタが画面中央に位置すべき絶対座標を計算する
    // スクロール判定の基準点として使用する
    std::pair<int,int> computeCharacterCenterAbsPos(int spriteW, int spriteH) const;

    // マップ外にスクロールしないように境界チェックを行う
    bool canScrollUp   (int viewOffsetY, int amountUp  ) const { return (viewOffsetY - amountUp  ) >= 0; }
    bool canScrollLeft (int viewOffsetX, int amountLeft) const { return (viewOffsetX - amountLeft) >= 0; }
    bool canScrollDown (int nextTopTileY, int screenTileCountY, int tileHeightNum) const { return nextTopTileY + screenTileCountY <= tileHeightNum; }
    bool canScrollRight(int nextTopTileX, int screenTileCountX, int tileWidthNum ) const { return nextTopTileX + screenTileCountX <= tileWidthNum;  }

    // キャラクタが画面中央を超えているかどうかの判定
    bool isCharacterAboveCenter  (int absCharaY, int charaYMax) const { return absCharaY <= charaYMax; }
    bool isCharacterBelowCenter  (int absCharaY, int charaYMax) const { return absCharaY >= charaYMax; }
    bool isCharacterLeftOfCenter (int absCharaX, int charaXMax) const { return absCharaX <= charaXMax; }
    bool isCharacterRightOfCenter(int absCharaX, int charaXMax) const { return absCharaX >= charaXMax; }
};
