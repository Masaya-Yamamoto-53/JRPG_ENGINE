#pragma once
#include "common.h"
#include "MovementStrategy.h"

class PlayerMovementStrategy : public MovementStrategy {
    MoveAmounts m_amounts;
    Direction m_direction;      // 向いている方向
    Direction m_prevDirection;  // 向いている方向（前回値）

public:
    void setMoveAmounts(const MoveAmounts& amounts) override;
    void setDirection(Direction direction) override;
    Movement computeMovement() override;
};
