#pragma once
#include "common.h"
#include "MovementStrategy.h"

class PlayerMovementStrategy : public MovementStrategy {
    MoveAmounts m_amounts;
    Direction m_direction;      // 向いている方向
    Direction m_prevDirection;  // 向いている方向（前回値）

public:
    Movement update(const MoveAmounts& amounts, Direction direction) override;
};
