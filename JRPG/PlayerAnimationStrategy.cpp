#include "PlayerAnimationStrategy.h"

PlayerAnimationStrategy::PlayerAnimationStrategy() {
    imgPattern = {
         0, -1, -1, -1, -1, -1, -1, -1,
        -1,  0,  0,  0,  0,  0,  0,  0,
         0,  1,  1,  1,  1,  1,  1,  1,
         1,  0,  0,  0,  0,  0,  0,  0
    };
}

