#pragma once
#include <functional>
#include "InputManager.h"
#include "Field.h"
#include "FieldCharacter.h"
#include "FieldRenderer.h"
#include "MovementController.h"

class FieldManager {
private:
    Field m_field;
    FieldRenderer m_renderer;
    MovementController m_movementController;

public:
    FieldManager();
    // キャラクタ設定
    void setCharacter(FieldCharacter* character);
    // フィールドとキャラクタの更新
    void update();
    // フィールドとキャラクタの描画
    void draw();
    // ステージデータ読み込み
    void load();
}
