#include "DxLib.h"
#include "FieldRenderer.h"
#include "DebugManager.h"

FieldRenderer::FieldRenderer() {
}

void FieldRenderer::drawField(const Field& field) {
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
              x + 32, y + spriteHeight / 2
            , x + spriteWidth - 32, y + spriteHeight
            , GetColor(0, 255, 0), FALSE
        );
    }
}
