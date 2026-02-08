#pragma once
#include "GameScene.h"

class SceneManager;

class TitleScene : public GameScene {
private:
    SceneManager* m_sceneManager;
    int m_selectedIndex;
public:
    TitleScene(SceneManager* sceneManager);
    void update() override;
    void render() override;
};

