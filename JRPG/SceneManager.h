#pragma once
#include <memory>
#include "GameScene.h"

#include "TitleScene.h"
#include "FieldScene.h"
#include "MenuScene.h"
#include "BattleScene.h"

#include "Party.h"

enum class SceneType {
      Title
    , Opening
    , Field
    , Menu
    , Battle
};

class SceneManager {
private:
    SceneType currentSceneType;
    std::unique_ptr<GameScene> currentScene;

    Party m_party;  // パーティ情報

public:
    SceneManager();
    void update();
    void render();
    // シーン切替
    void changeScene(SceneType type);

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

    Party& getParty() { return m_party; }

private:
    // シーン生成
    template <typename ... Args>
    std::unique_ptr<GameScene> createScene(SceneType type, Args&& ... args) {
        switch (type) {
        case SceneType::Title:
            return std::make_unique<TitleScene>(this, std::forward<Args>(args)...);
        case SceneType::Field:
            printf("Create Scene: Field\n");
            return std::make_unique<FieldScene>(this, std::forward<Args>(args)...);
        //case SceneType::Menu:
        //    return std::make_unique<MenuScene>(this, std::forward<Args>(args)...);
        //case SceneType::Battle:
        //    return std::make_unique<BattleScene>(this, std::forward<Args>(args)...);
        }
        return nullptr;
    }
};
