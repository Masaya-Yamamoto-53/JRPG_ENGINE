#pragma once
#include <string>
#include <vector>
#include "common.h"

class FieldCharacter {
private:
    // Constatnts
    static constexpr int CharacterSpriteNum = 27;
    static constexpr int RunStartFrame      = 30;

    static constexpr int WalkSpeed = 2;  // 歩行時の速度
    static constexpr int RunSpeed  = 4;  // 走行時の速度

    // Identity
    std::string m_id;

    // Position
    int m_x;
    int m_y;

    // Movement / State
    bool m_running;
    Direction m_prevDirection;

    // Animation
    int m_frame;       // 現在のアニメーションフレーム番号
    int m_runCounter;  // 走り始めるまでのカウンタ
    int m_animIndex;   // 描画用インデックス

    // Sprite info
    int m_spriteWidth;
    int m_spriteHeight;
    std::vector<int> m_images;  // 読み込んだ画像ハンドル

public:
    FieldCharacter(std::string id);

    int getMoveAmount();

    void update(
          const MoveAmounts & amounts
        , Direction direction);

    int getX() const;
    int getY() const;

    int getSpriteWidth() const;
    int getSpriteHeight() const;

    int getCurrentFrame() const;
    const std::vector<int>& getImages() const;

private:
    void loadImages();
};
