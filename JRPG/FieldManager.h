#pragma once
#include <functional>
#include "InputController.h"
#include "Field.h"
#include "FieldCharacter.h"
#include "FieldRenderer.h"
#include "MovementController.h"

class FieldManager {
private:
    InputController m_inputController;
    Field m_field;
    FieldRenderer m_renderer;
    MovementController m_movementController;

public:
    FieldManager();
    // フィールドとキャラクタの更新
    void update();
    // フィールドとキャラクタの描画
    void draw();
    // ステージデータ読み込み
    void load();
};
