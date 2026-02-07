#pragma once
#include <memory>
#include "GameScene.h"

#include "FieldScene.h"
#include "MenuScene.h"
#include "BattleScene.h"

enum class SceneType {
      Field
    , Menu
    , Battle
};

class SceneManager {
private:
    SceneType currentSceneType;
    std::unique_ptr<GameScene> currentScene;

public:
    SceneManager() {
        changeScene(SceneType::Field);
    }

    void update() {
        if (currentScene) {
            currentScene->update();
        }
    }

    void render() {
        if (currentScene) {
            currentScene->render();
        }
    }

    // シーン切替
    void changeScene(SceneType type) {
        currentScene = createScene(type);
        currentSceneType = type;
    }

    // 戦闘など、追加データが必要な場合の切替
    template <typename ... Args>
    void changeScene(SceneType type, Args&& ... args) {
        currentScene = createScene(type, std::forward<Args>(args) ...);
        currentSceneType = type;
    }

    /*
    SceneType getCurrentSceneType() const {
        return currentSceneType;
    }
    */

private:
    // シーン生成
    template <typename ... Args>
    std::unique_ptr<GameScene> createScene(SceneType type, Args&& ... args) {
        switch (type) {
        case SceneType::Field:
            return std::make_unique<FieldScene>(this, std::forward<Args>(args)...);
        //case SceneType::Menu:
        //    return std::make_unique<MenuScene>(this, std::forward<Args>(args)...);
        //case SceneType::Battle:
        //    return std::make_unique<BattleScene>(this, std::forward<Args>(args)...);
        }
        return nullptr;
    }
};
