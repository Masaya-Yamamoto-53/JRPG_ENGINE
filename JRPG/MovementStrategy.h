#pragma once
#include "common.h"

struct Movement {
    int dx;
    int dy;
    bool isMoving;
    Direction direction;
};

class MovementStrategy {
protected:
    static constexpr int RunStartFrame = 30;
    static constexpr int WalkSpeed = 2;  // 歩行時の速度
    static constexpr int RunSpeed  = 4;  // 走行時の速度
    int m_runCounter;  // 走り始めるまでのカウンタ
    bool m_running;    // 走っているかどうか

public:
    virtual ~MovementStrategy() = default;

    virtual bool isRunning() const = 0;
    virtual int getMoveAmount() const = 0;
    // キャラクタの移動量を計算
    virtual Movement update(const MoveAmounts& amounts, Direction direction) = 0;
};
