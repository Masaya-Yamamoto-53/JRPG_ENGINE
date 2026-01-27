#pragma once
#include <vector>
#include <string>
#include "common.h"

class ICharacterAnimation {
public:
    virtual ~ICharacterAnimation() = default;

    virtual void loadImages(const std::string& baseDir, const std::string& id) = 0;
    virtual const std::vector<int>& getImages() const = 0;
    virtual int getCurrentFrame() const = 0;
    virtual int getMoveAmount() const = 0;
    virtual void updateAnimation(Direction useDir, bool isMoving) = 0;
    virtual int calcAnimIndex(Direction dir) const = 0;
};

