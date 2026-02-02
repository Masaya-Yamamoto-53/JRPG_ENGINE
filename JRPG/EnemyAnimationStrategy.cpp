#include <fstream>
#include "DxLib.h"
#include "EnemyAnimationStrategy.h"

#include "json.hpp"
using json = nlohmann::json;

EnemyAnimationStrategy::EnemyAnimationStrategy()
    : m_running(false)
    , m_frame(0)
    , m_runCounter(0)
    , m_animIndex(1)
{
}

const CharacterImage& EnemyAnimationStrategy::getImage() const {
    return m_images[m_animIndex];
}

int EnemyAnimationStrategy::getMoveAmount() const {
    return (m_running ? RunSpeed : WalkSpeed);
}

void EnemyAnimationStrategy::updateAnimation(Direction useDir, bool isMoving) {
    if(isMoving) {
        m_runCounter = (std::min)(m_runCounter + 1, RunStartFrame);
        m_running = (m_runCounter >= RunStartFrame);
        m_frame = (m_frame + 1) % imgPattern.size();
    }
    else {
        m_runCounter = 0;
        m_running = false;
        m_frame = 0;
    }
    if (useDir != Direction::None) {
        m_animIndex = calcAnimIndex(useDir);
    }
}

int EnemyAnimationStrategy::calcAnimIndex(Direction dir) const {
    int base = 0;
    switch (dir) {
    case Direction::Down:
        base = m_down;
        break;
    case Direction::Up:
        base = m_up;
        break;
    case Direction::Left:
        base = m_running ? m_leftRun  : m_left;
        break;
    case Direction::Right:
        base = m_running ? m_rightRun : m_right;
        break;
    default:
        break;
    }
    return base + imgPattern[m_frame];
}

