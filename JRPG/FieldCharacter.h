#pragma once
#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "IFieldEntity.h"
#include "ICharacterAnimation.h"

class FieldCharacter : public IFieldEntity {
private:
    std::string m_id;       // キャラクタID
    std::string m_baseDir;  // ベースディレクトリ

    int m_x;  // X座標
    int m_y;  // Y座標

    Direction m_direction; //キャラクタの方向

    int m_spriteWidth;
    int m_spriteHeight;
    std::unique_ptr<ICharacterAnimation> m_anim;

public:
    FieldCharacter(
          std::string id
        , std::string baseDir
        , std::unique_ptr<ICharacterAnimation> anim
    );

    void update(
          const MoveAmounts & amounts
        , Direction direction);

    int getX() const;
    int getY() const;
    int getMoveAmount() const;

    int getSpriteWidth() const;
    int getSpriteHeight() const;

    int getCurrentFrame() const;
    const std::vector<int>& getImages() const;

};
