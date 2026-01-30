#include <stdio.h>
#include "EnemyMovementStrategy.h"

void EnemyMovementStrategy::setMoveAmounts(const MoveAmounts& m) {
    m_amounts = m;
}

Movement EnemyMovementStrategy::computeMovement() {
    Movement movement;

    // Movement amount
    int dy = (m_amounts.downFlag ? -m_amounts.down : 0)
           + (m_amounts.upFlag   ?  m_amounts.up   : 0);

    int dx = (m_amounts.leftFlag  ?  m_amounts.left  : 0)
           + (m_amounts.rightFlag ? -m_amounts.right : 0);

    movement.dx = dx;
    movement.dy = dy;
    movement.direction = Direction::Down;
    movement.isMoving = false;

    return movement;
}
