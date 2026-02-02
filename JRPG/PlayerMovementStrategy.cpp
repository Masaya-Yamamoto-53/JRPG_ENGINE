#include "PlayerMovementStrategy.h"

void PlayerMovementStrategy::setMoveAmounts(const MoveAmounts& m) {
    m_amounts = m;
}

void PlayerMovementStrategy::setDirection(Direction direction) {
    m_direction = direction;
}

Movement PlayerMovementStrategy::computeMovement() {
    // Movement amount
    int dy = (m_amounts.downFlag ?  m_amounts.down : 0)
           + (m_amounts.upFlag   ? -m_amounts.up   : 0);

    int dx = (m_amounts.leftFlag  ? -m_amounts.left  : 0)
           + (m_amounts.rightFlag ?  m_amounts.right : 0);

    // Update the animation counter based on the movement state
    const bool isMoving = (m_amounts.downFlag
                        || m_amounts.upFlag
                        || m_amounts.leftFlag
                        || m_amounts.rightFlag
                        );

    Direction useDir = (m_direction != Direction::None)
                     ? m_direction
                     : m_prevDirection;

    m_prevDirection = m_direction;

    return Movement{ dx, dy, isMoving, useDir};
}

