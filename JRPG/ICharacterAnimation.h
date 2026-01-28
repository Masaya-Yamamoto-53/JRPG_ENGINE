#pragma once
#include <vector>
#include <string>
#include "common.h"

struct CharacterImage {
    int handle;
    bool flip;
};

class ICharacterAnimation {
public:
    virtual ~ICharacterAnimation() = default;

    virtual void loadImages(const std::string& baseDir, const std::string& id) = 0;
    virtual const CharacterImage& getImage() const = 0;
    virtual int getMoveAmount() const = 0;
    virtual void updateAnimation(Direction useDir, bool isMoving) = 0;
    virtual int calcAnimIndex(Direction dir) const = 0;
    // スプライト画像の幅
    virtual int getSpriteWidth() const = 0;
    // スプライト画像の高さ
    virtual int getSpriteHeight() const = 0;
};

