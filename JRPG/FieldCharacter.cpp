#include <array>
#include "DxLib.h"
#include "FieldCharacter.h"
#include "GameSettings.h"

// Image pattern for animation frames
static constexpr std::array<int, 32> imgPattern = {
     0, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  1,  1,  1,  1,  1,  1,
     1,  0,  0,  0,  0,  0,  0,  0
};

FieldCharacter::FieldCharacter(std::string id)
    : m_id(id)
    , m_x(0)
    , m_y(0)
    , m_running(false)
    , m_prevDirection(Direction::None)
    , m_frame(0)
    , m_runCounter(0)
    , m_animIndex(1)
    , m_spriteWidth(GameSettings::instance().getSpriteWidth())
    , m_spriteHeight(GameSettings::instance().getSpriteHeight())
{
    loadImages();
}

int FieldCharacter::getMoveAmount() const {
    return (m_running ? RunSpeed : WalkSpeed);
}

void FieldCharacter::update(
      const MoveAmounts& amounts
    , Direction direction
    ) {
    // Movement amount
    int coordinateUpDw = (amounts.downFlag ?  amounts.down : 0)
                       + (amounts.upFlag   ? -amounts.up   : 0);

    int coordinateLtRt = (amounts.leftFlag  ? -amounts.left  : 0)
                       + (amounts.rightFlag ?  amounts.right : 0);

    // Update the animation counter based on the movement state
    const bool isMoving = (amounts.downFlag
                        || amounts.upFlag
                        || amounts.leftFlag
                        || amounts.rightFlag
                        );
    if (isMoving) {
        m_runCounter++;
        if (m_runCounter >= RunStartFrame) {
            m_runCounter = RunStartFrame;
            m_running = true;
        }
        m_frame = (m_frame + 1) % imgPattern.size();
    }
    else {
        m_runCounter = 0;
        m_running = false;
        m_frame = 0;
    }

    enum class Direction useDir = (direction != Direction::None)
                                ? direction
                                : m_prevDirection;

    if (useDir != Direction::None) {
        switch (useDir) {
        case Direction::Down:
            m_animIndex = 1;
            break;
        case Direction::Up:
            m_animIndex = 7;
            break;
        case Direction::Left:
            m_animIndex = m_running ? 16 :  4;
            break;
        case Direction::Right:
            m_animIndex = m_running ? 25 : 22;
            break;
        default:
            break;
        }
        m_animIndex += imgPattern[m_frame];
    }

    m_y += coordinateUpDw;
    m_x += coordinateLtRt;

    m_prevDirection = direction;
}

int FieldCharacter::getX() const { return m_x; }
int FieldCharacter::getY() const { return m_y; }
int FieldCharacter::getSpriteWidth() const { return m_spriteWidth; }
int FieldCharacter::getSpriteHeight() const { return m_spriteHeight; }
int FieldCharacter::getCurrentFrame() const { return m_animIndex; }
const std::vector<int>& FieldCharacter::getImages() const { return m_images; }

void FieldCharacter::loadImages() {
    SetTransColor(0, 255, 0);  // ìßâﬂêFÇê›íË

    std::string dirName = "assets\\characters\\players\\" + m_id + "\\";
    for (int i = 0; i < CharacterSpriteNum; i++) {
        char baseName[4];
        std::snprintf(baseName, sizeof(baseName), "%02d", i);
        std::string filePath = dirName + baseName + ".bmp";

        m_images.push_back(LoadGraph(filePath.c_str()));
    }
}
