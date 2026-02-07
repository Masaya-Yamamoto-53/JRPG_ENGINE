#pragma once
#include "GameScene.h"
class SceneManager;
#include "FieldManager.h"

class FieldScene : public GameScene {
private:
    SceneManager* m_sceneManager;
    std::unique_ptr<FieldManager> m_fieldManager;

public:
    FieldScene(SceneManager* sceneManager);
    void update() override;
    void render() override;

};
