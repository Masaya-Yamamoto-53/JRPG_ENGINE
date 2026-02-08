#pragma once
#include "SceneManager.h"

SceneManager::SceneManager()
    : m_party()
{
    m_party.addMember(0); // メンバーID 0 を追加
    changeScene(SceneType::Title);
}

void SceneManager::update() {
    if (currentScene) {
        currentScene->update();
    }
}

void SceneManager::render() {
    if (currentScene) {
        currentScene->render();
    }
}

// シーン切替
void SceneManager::changeScene(SceneType type) {
    printf("Change Scene: %d\n", type);
    currentScene = createScene(type);
    currentSceneType = type;
}
