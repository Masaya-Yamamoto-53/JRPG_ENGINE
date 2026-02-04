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

Movement EnemyMovementStrategy::update(const MoveAmounts& amounts, Direction direction) {
    // カメラ移動量を反映
    int dy = (amounts.downFlag  ? -amounts.down  : 0)
           + (amounts.upFlag    ?  amounts.up    : 0);

    int dx = (amounts.leftFlag  ?  amounts.left  : 0)
           + (amounts.rightFlag ? -amounts.right : 0);

    auto target = m_waypoints[m_currentIndex];
    int targetX = target.first;
    int targetY = target.second;


    // 敵のパトロール移動（AI）
    int aiDx = 0;
    int aiDy = 0;

    // X方向の移動
    if (m_posX < targetX) aiDx =  m_speed;
    if (m_posX > targetX) aiDx = -m_speed;

    // Y方向の移動
    if (m_posY < targetY) aiDy =  m_speed;
    if (m_posY > targetY) aiDy = -m_speed;

    // 目的地に到達したら次の waypoint へ
    if (abs(m_posX - targetX) < m_speed &&
        abs(m_posY - targetY) < m_speed) {
        m_currentIndex = (m_currentIndex + 1) % m_waypoints.size();
    }
 
    // 敵の内部位置更新
    m_posX += aiDx;
    m_posY += aiDy;

    // カメラ移動量 + AI移動量を合成
    Movement movement;
    movement.dx = dx + aiDx;
    movement.dy = dy + aiDy;
    movement.direction = m_direction;
    movement.isMoving = (aiDx != 0 || aiDy != 0);

    if (aiDx > 0) movement.direction = Direction::Right;
    else if (aiDx < 0) movement.direction = Direction::Left;
    else if (aiDy > 0) movement.direction = Direction::Down;
    else if (aiDy < 0) movement.direction = Direction::Up;
    else movement.direction = m_direction;  // 動いていない時は前の方向を維持

    m_direction = movement.direction;




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
