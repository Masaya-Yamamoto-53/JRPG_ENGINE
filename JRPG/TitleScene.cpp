#include "TitleScene.h"
#include "SceneManager.h"
#include "InputDeviceHub.h"

TitleScene::TitleScene(SceneManager* sceneManager)
    : m_sceneManager(sceneManager)
{
}

void TitleScene::update() {
    if (InputDeviceHub::instance().isKeyTriggered(InputDeviceHub::KEY_ENTER)) {
        // ƒV[ƒ“Ø‘Ö
        m_sceneManager->changeScene(SceneType::Field);
    }
}

void TitleScene::render() {
}
