#include "DxLib.h"
#include "FieldRenderer.h"
#include "GameSettings.h"
#include "DebugManager.h"

FieldRenderer::FieldRenderer()
{
}

void DrawDottedVLine(int x, int winH, int color) {
    const int segment = 4;   // 線の長さ
    const int gap     = 4;   // 空白の長さ

    for (int y = 0; y < winH; y += segment + gap) {
        DrawLine(x, y, x, y + segment, color);
    }
}

void DrawDottedHLine(int y, int winW, int color) {
    const int segment = 4;
    const int gap     = 4;

    for (int x = 0; x < winW; x += segment + gap) {
        DrawLine(x, y, x + segment, y, color);
    }
}

void FieldRenderer::drawField(const Field& field, int counter) {
    const TileMap& map = field.getTileMap();
    const TileSet& set = field.getTileSet();

    int tileH = set.getTileHeight();
    int tileW = set.getTileWidth();

    // タイルのSTART位置を算出
    int startY = field.getViewOffsetY() / tileH;
    int startX = field.getViewOffsetX() / tileW;

    int endY = GameSettings::instance().getWindowHeight() / tileH;
    int endX = GameSettings::instance().getWindowWidth()  / tileW;

    // 描画範囲を1タイル分余分に広げるための補正値
    // (viewOffset + tileSize - 1) / tileSize により、
    // 現在のビューオフセットが次のタイル境界を跨いでいるかどうかを判定し、
    // 必要なタイル数を1つ多めに描画する
    int offsetY = (field.getViewOffsetY() + tileH - 1) / tileH;
    int offsetX = (field.getViewOffsetX() + tileW - 1) / tileW;

    for (int y = startY ; y < endY + offsetY; y++) {
        for (int x = startX; x < endX + offsetX; x++) {
            auto index = map.get(x, y);
            int img = set.getTileImage(index.first, index.second, counter);

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
    if (DebugManager::instance().enabled()) {
        // タイル境界線の描画（デバッグ用）
        int winW = GameSettings::instance().getWindowWidth();
        int winH = GameSettings::instance().getWindowHeight();

        // 縦線（タイルのX境界）
        for (int x = 0; x <= winW; x += tileW) {
            int screenX = x - (field.getViewOffsetX() % tileW);
            DrawDottedVLine(screenX, winH, GetColor(0, 0, 255));
        }

        // 横線（タイルのY境界）
        for (int y = 0; y <= winH; y += tileH) {
            int screenY = y - (field.getViewOffsetY() % tileH);
            DrawDottedHLine(screenY, winW, GetColor(0, 0, 255));
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
              x + (spriteWidth / 4), y + (spriteHeight / 4) * 3
            , x + spriteWidth - (spriteWidth / 4), y + spriteHeight
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
