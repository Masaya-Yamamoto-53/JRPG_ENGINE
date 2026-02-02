#pragma once
#include <vector>
#include <string>
#include "common.h"

struct CharacterImage {
    int handle;
    int handleUpper;
    int handleLower;
    bool flip;
};

class AnimationStrategy {
protected:
    std::vector<CharacterImage> m_images;  // 読み込んだ画像ハンドル 

    int m_spriteWidth;   // スプライト画像の幅
    int m_spriteHeight;  // スプライト画像の高さ

    int m_up;
    int m_down;
    int m_left;
    int m_right;
    int m_leftRun;
    int m_rightRun;

public:
    virtual ~AnimationStrategy() = default;

    virtual void loadImages(const std::string& baseDir, const std::string& id);
    virtual const CharacterImage& getImage() const = 0;
    virtual int getMoveAmount() const = 0;
    virtual void updateAnimation(Direction useDir, bool isMoving) = 0;
    virtual int calcAnimIndex(Direction dir) const = 0;
    // スプライト画像の幅
    virtual int getSpriteWidth()  const { return m_spriteWidth;  }
    // スプライト画像の高さ
    virtual int getSpriteHeight() const { return m_spriteHeight; }

};

