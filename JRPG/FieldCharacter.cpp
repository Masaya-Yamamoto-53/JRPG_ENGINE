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
    : m_x(100)  // Žb’è
    , m_y(100)  // Žb’è
    , m_move(std::move(move))
    , m_anim(std::move(anim))
   
{
    m_anim->loadImages(baseDir, id);
}

void FieldCharacter::update(const MoveAmounts& amounts, Direction direction) {
    Movement movement = m_move->update(amounts, direction);
    m_anim->update(movement.direction, movement.isMoving, m_move->isRunning());
    m_y += movement.dy;
    m_x += movement.dx;
}

int FieldCharacter::getX() const { return m_x; }
int FieldCharacter::getY() const { return m_y; }
int FieldCharacter::getMoveAmount() const { return m_move->getMoveAmount(); }
int FieldCharacter::getSpriteWidth() const { return m_anim->getSpriteWidth(); }
int FieldCharacter::getSpriteHeight() const { return m_anim->getSpriteHeight(); }
const CharacterImage& FieldCharacter::getImage() const { return m_anim->getImage(); }
