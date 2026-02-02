#pragma once
#include <array>
#include "AnimationStrategy.h"

class EnemyAnimationStrategy : public AnimationStrategy {
private:
    static constexpr int RunStartFrame = 30;
    static constexpr int WalkSpeed = 2;  // 歩行時の速度
    static constexpr int RunSpeed  = 4;  // 走行時の速度

    // Image pattern for animation frames
    static constexpr std::array<int, 32> imgPattern = {
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1
    };

    int m_frame;       // 現在のアニメーションフレーム番号
    int m_runCounter;  // 走り始めるまでのカウンタ
    int m_animIndex;   // 描画用インデックス
    bool m_running;    // 走っているかどうか

public:
    EnemyAnimationStrategy();
    const CharacterImage& getImage() const override;
    int getMoveAmount() const override;
    void updateAnimation(Direction useDir, bool isMoving) override;
    int calcAnimIndex(Direction dir) const override;

};
#pragma once
