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
    virtual void setMoveAmounts(const MoveAmounts& amounts) {};
    virtual void setDirection(Direction direction) {};
    // キャラクタの移動量を計算
    virtual Movement computeMovement() = 0;
};
