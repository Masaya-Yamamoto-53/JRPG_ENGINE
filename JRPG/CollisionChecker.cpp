#include "CollisionChecker.h"

bool CollisionChecker::isWall(
      const TileSet& tileSet
    , const TileMap& tileMap
    , Direction dir
    , int absCharaX, int absCharaY
    , int spriteW, int spriteH
) const
{
    // 当たり判定の左右端
    int hitLeft   = absCharaX + spriteW / 4;
    int hitRight  = absCharaX + spriteW - spriteW / 4 - 1;

    // 当たり判定の上下端
    int hitTop = absCharaY;                   // キャラクタの頭頂部
    int hitBottom = absCharaY + spriteH - 1;  // キャラクタの足元

    int tileSizeX = tileSet.getTileWidth();
    int tileSizeY = tileSet.getTileHeight();

    // タイル座標（衝突判定用）
    int leftX  = hitLeft / tileSizeX;
    int rightX = hitRight / tileSizeX;
    int topY = (absCharaY + spriteH - spriteH / 4) / tileSizeY;  // キャラクタの下半身
    int btmY   = hitBottom / tileSizeY;

    int mapTilesY = tileMap.getTileHeightNum();
    int mapTilesX = tileMap.getTileWidthNum();

    switch (dir) {
    case Direction::Up:
        // ピクセルベース境界チェック
        if (hitTop < 0) return true;

        // タイル衝突判定
        return tileSet.isWall(tileMap.get(rightX, topY).first
                            , tileMap.get(rightX, topY).second)
            || tileSet.isWall(tileMap.get(leftX,  topY).first
                            , tileMap.get(leftX,  topY).second);

    case Direction::Down:
        // ピクセルベース境界チェック
        if (hitBottom >= mapTilesY * tileSizeY) return true;

        // タイル衝突判定
        return tileSet.isWall(tileMap.get(rightX, btmY).first
                            , tileMap.get(rightX, btmY).second)
            || tileSet.isWall(tileMap.get(leftX,  btmY).first
                            , tileMap.get(leftX,  btmY).second);
    case Direction::Left: 
        // ピクセルベース境界チェック
        if (hitLeft < 0) return true;

        // タイル衝突判定
        return tileSet.isWall(tileMap.get(leftX, topY).first
                            , tileMap.get(leftX, topY).second)
            || tileSet.isWall(tileMap.get(leftX, btmY).first
                            , tileMap.get(leftX, btmY).second);

    case Direction::Right:
        // ピクセルベース境界チェック
        if (hitRight >= mapTilesX * tileSizeX) return true;

        // タイル衝突判定
        return tileSet.isWall(tileMap.get(rightX, topY).first
                            , tileMap.get(rightX, topY).second)
            || tileSet.isWall(tileMap.get(rightX, btmY).first
                            , tileMap.get(rightX, btmY).second);
    }
    return false;
}