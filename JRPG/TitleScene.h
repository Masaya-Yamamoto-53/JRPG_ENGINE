#pragma once
#include "GameScene.h"
#include <vector>
#include <string>

class SceneManager;

struct MenuItem {
    std::string label;
    int normalHandle;
    int selectedHandle;
    int x;
    int y;
    std::string action;
};

class TitleScene : public GameScene {
private:
    SceneManager* m_sceneManager;
    int m_selectedIndex;
    std::vector<MenuItem> m_menuItems;
public:
    TitleScene(SceneManager* sceneManager);
    void update() override;
    void render() override;
    void load();
};

