#include "MovementController.h"
#include "GameSettings.h"

MoveAmounts MovementController::computeMoveAmounts(
      const DirectionalHoldFrames& holdFrames
    , int frameId
    , const IFieldEntity* entity
    , const Field& field
) {
    MoveAmounts amounts = { 0, 0, 0, 0, false, false, false, false };

    // キャラクタの絶対座標
    auto absPos = field.toAbsolute(entity->getX(), entity->getY());
    int absX = absPos.first;
    int absY = absPos.second;
    int spriteW = entity->getSpriteWidth();
    int spriteH = entity->getSpriteHeight();

    int maxMove = entity->getMoveAmount(); 

    // 移動可能量の計算
    if (holdFrames.up > holdFrames.down) {
        amounts.up    = computeMoveAmount(
            Direction::Up,    frameId, absX, absY,  0, -1, spriteW, spriteH, maxMove, field);
    }
    if (holdFrames.up < holdFrames.down) {
        amounts.down  = computeMoveAmount(
            Direction::Down,  frameId, absX, absY,  0, +1, spriteW, spriteH, maxMove, field);
    }
    if (holdFrames.left > holdFrames.right) {
        amounts.left  = computeMoveAmount(
            Direction::Left,  frameId, absX, absY, -1,  0, spriteW, spriteH, maxMove, field);
    }
    if (holdFrames.left < holdFrames.right) {
        amounts.right = computeMoveAmount(
            Direction::Right, frameId, absX, absY,  1,  0, spriteW, spriteH, maxMove, field);
    }

    // 移動フラグ
    amounts.upFlag    = amounts.up    > 0;
    amounts.downFlag  = amounts.down  > 0;
    amounts.leftFlag  = amounts.left  > 0;
    amounts.rightFlag = amounts.right > 0;

    return amounts;
}

Direction MovementController::computeDirection(
      const DirectionalHoldFrames& holdFrames
    , const MoveAmounts& amounts
) {
    Direction direction = Direction::None;

    // キャラクタが静止している場合、最後の向きを維持する
    direction = Direction::None;
    if (holdFrames.up > holdFrames.down) {
        direction = Direction::Up;
    }
    if (holdFrames.up < holdFrames.down) {
        direction = Direction::Down;
    }
    if (holdFrames.left > holdFrames.right) {
        direction = Direction::Left;
    }
    if (holdFrames.left < holdFrames.right) {
        direction = Direction::Right;
    }

    // 移動した方向を優先する
    if (amounts.up       > 0) { direction = Direction::Up;    }
    if (amounts.down     > 0) { direction = Direction::Down;  }
    if (amounts.left     > 0) { direction = Direction::Left;  }
    if (amounts.right    > 0) { direction = Direction::Right; }

    return direction;
}

int MovementController::computeMoveAmount(
      Direction dir
    , int frameId
    , int baseX, int baseY
    , int deltaX, int deltaY
    , int spriteW, int spriteH
    , int maxMove
    , const Field& field
    ) const {

    int tileW = GameSettings::instance().getFieldTileWidth();
    int tileH = GameSettings::instance().getFieldTileHeight();
    while (maxMove > 0) {
        int nextX = baseX + deltaX * maxMove;
        int nextY = baseY + deltaY * maxMove;
        if (!field.isWall(dir, frameId, nextX, nextY, tileW, tileH, spriteW, spriteH)) {
            return maxMove;  // 移動可能量を返す
        }
        maxMove--;
    }
    return 0;
}

