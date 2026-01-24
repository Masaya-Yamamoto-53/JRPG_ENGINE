#pragma once
#include <functional>
#include "Field.h"
#include "FieldCharacter.h"
#include "FieldRenderer.h"

struct DurationInputs {
    int up;
    int down;
    int left;
    int right;
};

class FieldManager {
private:
    Field m_field;
    FieldCharacter* m_character;
    FieldRenderer m_renderer;
    int m_frameCount;
    int m_animationCounter;
public:
    FieldManager();

    void setCharacter(FieldCharacter* character);

    void update();
    void draw();

    void load();
private:
    // 入力継続時間の取得
    DurationInputs computeDurationInputs();
    // 衝突判定と移動可能量の計算
    int computeMoveAmount(
          int baseX, int baseY
        , int deltaX, int deltaY
        , std::function<bool(int, int, int, int)> isWallFunc
    );
    // 移動可能量の計算
    MoveAmounts computeAmounts(const DurationInputs& durations, int absCharaX, int absCharaY);
    // 移動方向の決定
    Direction computeDirection(const DurationInputs& durations);
    // アニメーション更新
    void updateAnimation();
};
