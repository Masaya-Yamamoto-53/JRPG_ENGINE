#include "DxLib.h"
#include "InputDeviceHub.h"

InputDeviceHub& InputDeviceHub::instance() {
    static InputDeviceHub instance;
    return instance;
}

InputDeviceHub::InputDeviceHub()
    : m_keyboard()  // KeyboardInput‚Ì‰Šú‰»
{
}

void InputDeviceHub::update() {
    m_keyboard.update();
}

bool InputDeviceHub::isKeyTriggered(int keyIndex) const {
    return m_keyboard.isTriggered(keyIndex);
}

int InputDeviceHub::getKeyHoldFrames(int keyIndex) const {
    return m_keyboard.getHoldFrames(keyIndex);
}
