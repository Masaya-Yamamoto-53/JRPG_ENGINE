#include <iostream>
#include "EnemyMovementStrategy.h"

EnemyMovementStrategy::EnemyMovementStrategy(int minX, int maxX, int speed)
    : m_minX(minX)
    , m_maxX(maxX)
    , m_direction(Direction::Right)
    , m_speed(speed)
{
}

bool EnemyMovementStrategy::isRunning() const {
    return m_running;
}

int EnemyMovementStrategy::getMoveAmount() const {
    return (m_running ? RunSpeed : WalkSpeed);
}

Movement EnemyMovementStrategy::update(const MoveAmounts &amounts, Direction direction) {
    // カメラ移動量を反映
    int dy = (amounts.downFlag  ? -amounts.down  : 0)
           + (amounts.upFlag    ?  amounts.up    : 0);

    int dx = (amounts.leftFlag  ?  amounts.left  : 0)
           + (amounts.rightFlag ? -amounts.right : 0);

    // 敵のパトロール移動（AI）
    int aiDx = 0;
    int aiDy = 0;

    if (m_direction == Direction::Right) {
        aiDx = m_speed;
        if (m_posX + aiDx > m_maxX) {
            aiDx = 0;
            m_direction = Direction::Left;
        }
    }
    else if (m_direction == Direction::Left) {
        aiDx = -m_speed;
        if (m_posX + aiDx < m_minX) {
            aiDx = 0;
            m_direction = Direction::Right;
        }
    }

    // 敵の内部位置更新
    m_posX += aiDx;

    // カメラ移動量 + AI移動量を合成
    Movement movement;
    movement.dx = dx + aiDx;
    movement.dy = dy + aiDy;
    movement.direction = m_direction;
    movement.isMoving = (aiDx != 0 || aiDy != 0);

    if(movement.isMoving) {
        m_runCounter = (std::min)(m_runCounter + 1, RunStartFrame);
        m_running = (m_runCounter >= RunStartFrame);
    }
    else {
        m_runCounter = 0;
        m_running = false;
    }

    return movement;
}
