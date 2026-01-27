#include "DxLib.h"
#include "PlayerAnimation.h"

PlayerAnimation::PlayerAnimation()
    : m_running(false)
    , m_frame(0)
    , m_runCounter(0)
    , m_animIndex(1)
{
}

void PlayerAnimation::loadImages(const std::string& baseDir, const std::string& id) {
    SetTransColor(0, 255, 0);  // ìßâﬂêFÇê›íË

    
    std::string dirName = baseDir + id + "\\";
    for (int i = 0; i < CharacterSpriteNum; i++) {
        char baseName[4];
        std::snprintf(baseName, sizeof(baseName), "%02d", i);
        std::string filePath = dirName + baseName + ".bmp";

        m_images.push_back(LoadGraph(filePath.c_str()));
    }
}

int PlayerAnimation::getCurrentFrame() const { return m_animIndex; }

const std::vector<int>& PlayerAnimation::getImages() const {
    return m_images;
}

int PlayerAnimation::getMoveAmount() const {
    return (m_running ? RunSpeed : WalkSpeed);
}

void PlayerAnimation::updateAnimation(Direction useDir, bool isMoving) {
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

int PlayerAnimation::calcAnimIndex(Direction dir) const {
    int base = 0;
    switch (dir) {
    case Direction::Down:
        base = 1;
        break;
    case Direction::Up:
        base = 7;
        break;
    case Direction::Left:
        base = m_running ? 16 :  4;
        break;
    case Direction::Right:
        base = m_running ? 25 : 22;
        break;
    default:
        break;
    }
    return base + imgPattern[m_frame];
}
