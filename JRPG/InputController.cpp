#include "InputController.h"
#include "InputDeviceHub.h"

DirectionalHoldFrames InputController::getDirectionalHoldFrames() const {
    DirectionalHoldFrames frames;

    frames.up    = InputDeviceHub::instance().getKeyHoldFrames(KEY_INPUT_W);
    frames.down  = InputDeviceHub::instance().getKeyHoldFrames(KEY_INPUT_S);
    frames.left  = InputDeviceHub::instance().getKeyHoldFrames(KEY_INPUT_A);
    frames.right = InputDeviceHub::instance().getKeyHoldFrames(KEY_INPUT_D);

    return frames;
}
