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
