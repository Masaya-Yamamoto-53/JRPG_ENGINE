#include "FieldScene.h"
#include "FieldManager.h"

FieldScene::FieldScene(SceneManager* sceneManager)
    : m_sceneManager(sceneManager)
    , m_fieldManager(std::make_unique<FieldManager>())
{
}

void FieldScene::update() {
    m_fieldManager->update();
}

void FieldScene::render() {
    m_fieldManager->render();
}