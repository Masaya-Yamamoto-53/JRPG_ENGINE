#pragma once
#include "InputDeviceHub.h"

struct DirectionalHoldFrames {
    int up;
    int down;
    int left;
    int right;
};

class InputController {
public:
    // “ü—Í‚ÌŽæ“¾
    DirectionalHoldFrames getDirectionalHoldFrames() const;
};
