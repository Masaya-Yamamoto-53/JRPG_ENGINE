#include <array>
#include "DxLib.h"
#include "FieldCharacter.h"
#include "GameSettings.h"

FieldCharacter::FieldCharacter(
      std::string id
    , std::string baseDir
    , std::unique_ptr<MovementStrategy> move
    , std::unique_ptr<AnimationStrategy> anim
    )
    : m_x(0)
    , m_y(0)
    , m_move(std::move(move))
    , m_anim(std::move(anim))
   
{
    m_anim->loadImages(baseDir, id);
}

void FieldCharacter::update() {
    Movement movement = m_move->computeMovement();
    m_anim->updateAnimation(movement.direction, movement.isMoving);
    m_y += movement.dy;
    m_x += movement.dx;
}

void FieldCharacter::setDirection(Direction direction) {
    m_move->setDirection(direction);
}

void FieldCharacter::setMoveAmounts(const MoveAmounts& amounts) {
    m_move->setMoveAmounts(amounts);
}

int FieldCharacter::getX() const { return m_x; }
int FieldCharacter::getY() const { return m_y; }
int FieldCharacter::getMoveAmount() const { return m_anim->getMoveAmount(); }
int FieldCharacter::getSpriteWidth() const { return m_anim->getSpriteWidth(); }
int FieldCharacter::getSpriteHeight() const { return m_anim->getSpriteHeight(); }
const CharacterImage& FieldCharacter::getImage() const { return m_anim->getImage(); }
