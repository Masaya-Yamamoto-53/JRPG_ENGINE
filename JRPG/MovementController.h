#pragma once
#include "Field.h"
#include "IFieldEntity.h"
#include "InputManager.h"

class MovementController {
public:
    // Õ“Ë”»’è‚ÆˆÚ“®‰Â”\—Ê‚ÌŒvZ
    MoveAmounts computeMoveAmounts(
          const DirectionalHoldFrames& holdFrames
        , const IFieldEntity& entity
        , const Field& field
    );
    // ˆÚ“®•ûŒü‚ÌŒˆ’è
    Direction computeDirection(
          const DirectionalHoldFrames& holdFrames
        , const MoveAmounts& amounts
    );

private:
    int computeMoveAmount(
          Direction dir
        , int baseX, int baseY
        , int deltaX, int deltaY
        , int maxMove
        , const Field& field
        ) const;
};
