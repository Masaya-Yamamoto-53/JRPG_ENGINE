#pragma once
#include <array>
#include "ICharacterAnimation.h"

class PlayerAnimation : public ICharacterAnimation {
private:
    static constexpr int CharacterSpriteNum = 27;
    static constexpr int RunStartFrame      = 30;
    static constexpr int WalkSpeed = 2;  // 歩行時の速度
    static constexpr int RunSpeed  = 4;  // 走行時の速度

    // Image pattern for animation frames
    static constexpr std::array<int, 32> imgPattern = {
         0, -1, -1, -1, -1, -1, -1, -1,
        -1,  0,  0,  0,  0,  0,  0,  0,
         0,  1,  1,  1,  1,  1,  1,  1,
         1,  0,  0,  0,  0,  0,  0,  0
    };

    std::vector<CharacterImage> m_images;  // 読み込んだ画像ハンドル 

    int m_frame;       // 現在のアニメーションフレーム番号
    int m_runCounter;  // 走り始めるまでのカウンタ
    int m_animIndex;   // 描画用インデックス

    bool m_running;

    int m_spriteWidth;   // スプライト画像の幅
    int m_spriteHeight;  // スプライト画像の高さ

public:
    PlayerAnimation();
    void loadImages(const std::string& baseDir, const std::string& id) override;
    const CharacterImage& getImage() const override;
    int getMoveAmount() const override;
    void updateAnimation(Direction useDir, bool isMoving) override;
    int calcAnimIndex(Direction dir) const override;
    // スプライト画像の幅
    int getSpriteWidth() const;
    // スプライト画像の高さ
    int getSpriteHeight() const;
};
