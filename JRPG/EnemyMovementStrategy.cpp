#include <stdio.h>
#include "EnemyMovementStrategy.h"

EnemyMovementStrategy::EnemyMovementStrategy(int minX, int maxX, int speed)
    : m_minX(minX)
    , m_maxX(maxX)
    , m_direction(Direction::Right)
    , m_speed(speed)
{
}

void EnemyMovementStrategy::setPosition(int x, int y) {
    m_posX = x;
    m_posY = y;
}

void EnemyMovementStrategy::setMoveAmounts(const MoveAmounts& m) {
    m_amounts = m;
}

Movement EnemyMovementStrategy::computeMovement() {
    Movement movement;

    // カメラ移動量を反映
    int dy = (m_amounts.downFlag ? -m_amounts.down : 0)
           + (m_amounts.upFlag   ?  m_amounts.up   : 0);

    int dx = (m_amounts.leftFlag  ?  m_amounts.left  : 0)
           + (m_amounts.rightFlag ? -m_amounts.right : 0);

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
    movement.dx = dx + aiDx;
    movement.dy = dy + aiDy;
    movement.direction = m_direction;
    movement.isMoving = (aiDx != 0 || aiDy != 0);

    return movement;
}
