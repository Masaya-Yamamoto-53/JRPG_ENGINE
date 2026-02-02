#pragma once
#include <array>
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
    std::array<int, 32> imgPattern{};
    std::vector<CharacterImage> m_images;  // 読み込んだ画像ハンドル 

    int m_spriteWidth;   // スプライト画像の幅
    int m_spriteHeight;  // スプライト画像の高さ

    int m_up       = -1;
    int m_down     = -1;
    int m_left     = -1;
    int m_right    = -1;
    int m_leftRun  = -1;
    int m_rightRun = -1;

    int m_frame;       // 現在のアニメーションフレーム番号
    int m_animIndex;   // 描画用インデックス

protected:
    AnimationStrategy();

public:
    virtual ~AnimationStrategy() = default;

    virtual void loadImages(const std::string& baseDir, const std::string& id);
    virtual const CharacterImage& getImage() const;
    virtual void update(Direction useDir, bool isMoving, bool running);
    virtual int calcAnimIndex(Direction dir, bool running) const;

    // スプライト画像の幅
    virtual int getSpriteWidth()  const { return m_spriteWidth;  }
    // スプライト画像の高さ
    virtual int getSpriteHeight() const { return m_spriteHeight; }

};

