#pragma once

class IFieldEntity {
public:
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual int getSpriteWidth() const = 0;
    virtual int getSpriteHeight() const = 0;
    virtual int getMoveAmount() const = 0;
};
