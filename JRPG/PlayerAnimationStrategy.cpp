#include <fstream>
#include "DxLib.h"
#include "PlayerAnimationStrategy.h"

#include "json.hpp"
using json = nlohmann::json;

PlayerAnimationStrategy::PlayerAnimationStrategy()
    : m_running(false)
    , m_frame(0)
    , m_runCounter(0)
    , m_animIndex(1)
{
}

const CharacterImage& PlayerAnimationStrategy::getImage() const {
    return m_images[m_animIndex];
}

int PlayerAnimationStrategy::getMoveAmount() const {
    return (m_running ? RunSpeed : WalkSpeed);
}

void PlayerAnimationStrategy::update(Direction useDir, bool isMoving) {
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

int PlayerAnimationStrategy::calcAnimIndex(Direction dir) const {
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

