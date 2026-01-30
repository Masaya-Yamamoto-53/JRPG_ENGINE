#pragma once
#include "common.h"
#include "MovementStrategy.h"

class EnemyMovementStrategy : public MovementStrategy {
    MoveAmounts m_amounts;
public:
    void setMoveAmounts(const MoveAmounts& m) override;
    Movement computeMovement() override;
};

