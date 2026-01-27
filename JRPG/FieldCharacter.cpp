#include <array>
#include "DxLib.h"
#include "FieldCharacter.h"
#include "GameSettings.h"

FieldCharacter::FieldCharacter(
      std::string id
    , std::string baseDir
    , std::unique_ptr<ICharacterAnimation> anim
    )
    : m_id(id)
    , m_baseDir(baseDir)
    , m_x(0)
    , m_y(0)
    , m_direction(Direction::None)
    , m_spriteWidth(GameSettings::instance().getSpriteWidth())
    , m_spriteHeight(GameSettings::instance().getSpriteHeight())
    , m_anim(std::move(anim))
   
{
    m_anim->loadImages(baseDir, id);
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

    Direction useDir = (direction != Direction::None)
                                ? direction
                                : m_direction;

    m_anim->updateAnimation(useDir, isMoving);

    m_y += coordinateUpDw;
    m_x += coordinateLtRt;

    m_direction = direction;
}

int FieldCharacter::getX() const { return m_x; }
int FieldCharacter::getY() const { return m_y; }
int FieldCharacter::getMoveAmount() const { return m_anim->getMoveAmount(); }
int FieldCharacter::getSpriteWidth() const { return m_spriteWidth; }
int FieldCharacter::getSpriteHeight() const { return m_spriteHeight; }
int FieldCharacter::getCurrentFrame() const { return m_anim->getCurrentFrame(); }
const std::vector<int>& FieldCharacter::getImages() const { return m_anim->getImages(); }
