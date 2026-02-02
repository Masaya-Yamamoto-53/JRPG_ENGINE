#pragma once
#include "common.h"

struct Movement {
    int dx;
    int dy;
    bool isMoving;
    Direction direction;
};

class MovementStrategy {
private:

public:
    virtual ~MovementStrategy() = default;
    // キャラクタの移動量を計算
    virtual Movement update(const MoveAmounts& amounts, Direction direction) = 0;
};
