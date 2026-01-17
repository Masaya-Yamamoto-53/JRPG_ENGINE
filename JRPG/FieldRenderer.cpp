#include "DxLib.h"
#include "FieldRenderer.h"
#include "GameSettings.h"
#include "DebugManager.h"

FieldRenderer::FieldRenderer()
{
}

void FieldRenderer::drawField(const Field& field) {
    const TileMap& map = field.getTileMap();
    const TileSet& set = field.getTileSet();

    int tileH = set.getTileHeight();
    int tileW = set.getTileWidth();

    // タイルのSTART位置を算出
    int startY = field.getViewOffsetY() / tileH;
    int startX = field.getViewOffsetX() / tileW;

    int endY = GameSettings::instance().getWindowHeight() / tileH;
    int endX = GameSettings::instance().getWindowWidth()  / tileW;

    // Calculate the coordinates of the image positioned at the screen's origin (x, y = 0, 0).
    // startPixelY and startPixelX represent the coordinates of the image located at the screen’s origin.
    // PixelSize - 1 is used to perform rounding up, equivalent to the ceil function.
    // Finally, dividing by PixelSize determines the size of the image to be loaded.
    int offsetY = (field.getViewOffsetY() + tileH - 1) / tileH;
    int offsetX = (field.getViewOffsetX() + tileW - 1) / tileW;

    for (int y = startY ; y < endY + offsetY; y++) {
        for (int x = startX; x < endX + offsetX; x++) {
            int tileId = map.get(x, y);
            int img = set.getTileImage(tileId);

            if (img != -1) {
                DrawGraph(
                      (x * tileW) - field.getViewOffsetX()
                    , (y * tileH) - field.getViewOffsetY()
                    , img
                    , FALSE
                );
            }
        }
    }
}

void FieldRenderer::drawCharacter(const FieldCharacter& character) {
    // キャラの現在位置
    int x = character.getX();
    int y = character.getY();

    int spriteWidth  = character.getSpriteWidth();
    int spriteHeight = character.getSpriteHeight();

    // 現在のアニメーションフレーム
    int frame = character.getCurrentFrame();

    // スプライト画像
    const std::vector<int>& images = character.getImages();

    DrawGraph(x, y, images[frame], TRUE);

    if (DebugManager::instance().enabled()) {
        // スプライトの大きさを赤枠を表示
        DrawBox(
              x, y
            , x + spriteWidth, y + spriteHeight
            , GetColor(255, 0, 0), FALSE
        );
        // 当たり判定の閾値の目標位置を緑枠で表示
        DrawBox(
              x + 32, y + (spriteHeight / 4) * 3
            , x + spriteWidth - 32, y + spriteHeight
            , GetColor(0, 255, 0), FALSE
        );

        // 画面の中心線
        DrawLine(
              GameSettings::instance().getWindowWidth() / 2
            , 0
            , GameSettings::instance().getWindowWidth() / 2
            , GameSettings::instance().getWindowHeight()
            , GetColor(0, 0, 0)
            , FALSE
        );

        DrawLine(
              0
            , GameSettings::instance().getWindowHeight() / 2
            , GameSettings::instance().getWindowWidth()
            , GameSettings::instance().getWindowHeight() / 2
            , GetColor(0, 0, 0)
            , FALSE
        );
    }
}
