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

    int m_up       = -1;
    int m_down     = -1;
    int m_left     = -1;
    int m_right    = -1;
    int m_leftRun  = -1;
    int m_rightRun = -1;

public:
    virtual ~AnimationStrategy() = default;

    virtual void loadImages(const std::string& baseDir, const std::string& id);
    virtual const CharacterImage& getImage() const = 0;
    virtual int getMoveAmount() const = 0;
    virtual void update(Direction useDir, bool isMoving) = 0;
    virtual int calcAnimIndex(Direction dir) const = 0;
    // スプライト画像の幅
    virtual int getSpriteWidth()  const { return m_spriteWidth;  }
    // スプライト画像の高さ
    virtual int getSpriteHeight() const { return m_spriteHeight; }

};

