#include "DxLib.h"
#include "FieldRenderer.h"

FieldRenderer::FieldRenderer() {
}

void FieldRenderer::drawField(const Field& field) {
}

void FieldRenderer::drawCharacter(const FieldCharacter& character) {
    // キャラの現在位置
    int x = character.getX();
    int y = character.getY();

    // 現在のアニメーションフレーム
    int frame = character.getCurrentFrame();

    // スプライト画像
    const std::vector<int>& images = character.getImages();

    DrawGraph(x, y, images[frame], TRUE);
}
