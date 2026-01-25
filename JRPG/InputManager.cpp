#include "DxLib.h"
#include "InputManager.h"

InputManager& InputManager::instance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
    : m_keyboard()  // KeyboardInput‚Ì‰Šú‰»
{
}

void InputManager::update() {
    m_keyboard.update();
}

bool InputManager::isKeyTriggered(int keyIndex) const {
    return m_keyboard.isTriggered(keyIndex);
}

int InputManager::getKeyHoldFrames(int keyIndex) const {
    return m_keyboard.getHoldFrames(keyIndex);
}

DirectionalHoldFrames InputManager::getDirectionalHoldFrames() {
    DirectionalHoldFrames frames;

    frames.up    = getKeyHoldFrames(KEY_INPUT_W);
    frames.down  = getKeyHoldFrames(KEY_INPUT_S);
    frames.left  = getKeyHoldFrames(KEY_INPUT_A);
    frames.right = getKeyHoldFrames(KEY_INPUT_D);

    return frames;
}

