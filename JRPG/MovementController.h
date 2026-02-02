#pragma once
#include "Field.h"
#include "IFieldEntity.h"
#include "InputController.h"
#include "CollisionChecker.h"

class MovementController {
    CollisionChecker m_collisionChecker;

public:
    MovementController();

    // 入力とフィールド情報から移動量を計算
    MoveAmounts computeMoveAmounts(
          const DirectionalHoldFrames& holdFrames
        , const IFieldEntity* entity
        , const Field& field
    );
    // 入力と移動量からキャラクタの向きを決定
    Direction computeDirection(
          const DirectionalHoldFrames& holdFrames
        , const MoveAmounts& amounts
    );

private:
    // 指定方向に最大どれだけ移動できるかを計算
    int computeMoveAmount(
          Direction dir
        , int baseX, int baseY
        , int deltaX, int deltaY
        , int spriteW, int spriteH
        , int maxMove
        , const Field& field
        ) const;
};
