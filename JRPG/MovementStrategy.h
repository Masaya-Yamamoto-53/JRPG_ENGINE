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
    virtual void setMoveAmounts(const MoveAmounts& amounts) = 0;
    virtual void setDirection(Direction direction) = 0;
    // キャラクタの移動量を計算
    virtual Movement computeMovement() = 0;
};
