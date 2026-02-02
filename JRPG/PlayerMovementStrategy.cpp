#include "PlayerMovementStrategy.h"

Movement PlayerMovementStrategy::update(const MoveAmounts &amounts, Direction direction) {
    // Movement amount
    int dy = (amounts.downFlag  ?  amounts.down  : 0)
           + (amounts.upFlag    ? -amounts.up    : 0);

    int dx = (amounts.leftFlag  ? -amounts.left  : 0)
           + (amounts.rightFlag ?  amounts.right : 0);

    // Update the animation counter based on the movement state
    const bool isMoving = (amounts.downFlag
                        || amounts.upFlag
                        || amounts.leftFlag
                        || amounts.rightFlag
                        );

    Direction useDir = (direction != Direction::None)
                     ? direction
                     : m_prevDirection;

    m_prevDirection = direction;

    return Movement{ dx, dy, isMoving, useDir};
}

