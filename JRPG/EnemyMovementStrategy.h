#pragma once
#include "common.h"
#include "MovementStrategy.h"

class EnemyMovementStrategy : public MovementStrategy {
    MoveAmounts m_amounts;
    int m_minX;
    int m_maxX;
    int m_speed;

    int m_posX = 0;
    int m_posY = 0;

    Direction m_direction;

public:
    EnemyMovementStrategy(int minX, int maxX, int speed);
    Movement update(const MoveAmounts& amounts, Direction direction) override;
};

