#include "EnemyAnimationStrategy.h"

EnemyAnimationStrategy::EnemyAnimationStrategy() {
    imgPattern = {
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1
    };
}
/*
const CharacterImage& EnemyAnimationStrategy::getImage() const {
    return m_images[m_animIndex];
}

void EnemyAnimationStrategy::update(Direction useDir, bool isMoving, bool running) {
    if(isMoving) {
        m_frame = (m_frame + 1) % imgPattern.size();
    }
    else {
        m_frame = 0;
    }
    if (useDir != Direction::None) {
        m_animIndex = calcAnimIndex(useDir, running);
        m_animIndex += imgPattern[m_frame];
    }
}

int EnemyAnimationStrategy::calcAnimIndex(Direction dir, bool running) const {
    int base = 0;
    switch (dir) {
    case Direction::Down:
        base = m_down;
        break;
    case Direction::Up:
        base = m_up;
        break;
    case Direction::Left:
        base = running ? m_leftRun  : m_left;
        break;
    case Direction::Right:
        base = running ? m_rightRun : m_right;
        break;
    default:
        break;
    }
    return base;
}
*/
