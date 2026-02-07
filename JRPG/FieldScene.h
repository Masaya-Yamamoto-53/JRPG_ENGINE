#pragma once
#include "GameScene.h"
class SceneManager;

#include "InputController.h"
#include "Field.h"
#include "FieldRenderer.h"
#include "MovementController.h"

class FieldScene : public GameScene {
private:
    SceneManager* m_sceneManager;

    InputController m_inputController;
    Field m_field;
    FieldRenderer m_renderer;
    MovementController m_movementController;

public:
    FieldScene(SceneManager* sceneManager);
    // フィールドとキャラクタの更新
    void update() override;
    // フィールドとキャラクタの描画
    void render() override;
    // ステージデータ読み込み
    void load();
};
