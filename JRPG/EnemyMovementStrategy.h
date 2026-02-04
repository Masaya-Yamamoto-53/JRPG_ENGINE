#pragma once
#include <vector>
#include "common.h"
#include "MovementStrategy.h"

class EnemyMovementStrategy : public MovementStrategy {
    int m_minX;
    int m_maxX;
    int m_speed;

    int m_posX = 0;
    int m_posY = 0;

    Direction m_direction;

    std::vector<std::pair<int, int>> m_waypoints = {
        {100, 100},
        {200, 100},
    };

    int m_currentIndex = 0;

public:
    EnemyMovementStrategy(int minX, int maxX, int speed);
    bool isRunning() const;
    int getMoveAmount() const;
    Movement update(const MoveAmounts& amounts, Direction direction) override;
};

