#include "MovementController.h"

MovementController::MovementController()
    : m_collisionChecker()
{
}

MoveAmounts MovementController::computeMoveAmounts(
      const DirectionalHoldFrames& holdFrames
    , const FieldCharacter* chara
    , const Field& field
) {
    MoveAmounts amounts = { 0, 0, 0, 0, false, false, false, false };

    // キャラクタの絶対座標
    auto absPos = field.getCamera().toAbsolute(chara->getX(), chara->getY());
    int absX = absPos.first;
    int absY = absPos.second;

    int spriteW = chara->getSpriteWidth();
    int spriteH = chara->getSpriteHeight();
    int maxMove = chara->getMoveAmount(); 

    // 移動可能量の計算
    if (holdFrames.up > holdFrames.down) {
        amounts.up    = computeMoveAmount(
            Direction::Up,    absX, absY,  0, -1, spriteW, spriteH, maxMove, field);
    }
    if (holdFrames.up < holdFrames.down) {
        amounts.down  = computeMoveAmount(
            Direction::Down,  absX, absY,  0, +1, spriteW, spriteH, maxMove, field);
    }
    if (holdFrames.left > holdFrames.right) {
        amounts.left  = computeMoveAmount(
            Direction::Left,  absX, absY, -1,  0, spriteW, spriteH, maxMove, field);
    }
    if (holdFrames.left < holdFrames.right) {
        amounts.right = computeMoveAmount(
            Direction::Right, absX, absY, +1,  0, spriteW, spriteH, maxMove, field);
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

    // --- 基本方針 ---
    // 1. まず「どの方向キーがより長く押されているか」で基準方向を決める
    //    （反対方向が同時押しの場合は、先に押された方＝holdFrames が大きい方を優先）
    // 2. その後、実際に移動可能な方向（amounts > 0）があれば、そちらを最優先する
    //    （上下＋左右が同時に押されている場合は、左右が動けるなら左右を優先）

    // 押下フレーム数による基準方向の決定
    direction = Direction::None;
    if (holdFrames.up > holdFrames.down)    { direction = Direction::Up;    }
    if (holdFrames.up < holdFrames.down)    { direction = Direction::Down;  }
    if (holdFrames.left > holdFrames.right) { direction = Direction::Left;  }
    if (holdFrames.left < holdFrames.right) { direction = Direction::Right; }

    // 実際に移動可能な方向があれば、それを最優先する
    if (amounts.up    > 0) { direction = Direction::Up;    }
    if (amounts.down  > 0) { direction = Direction::Down;  }
    if (amounts.left  > 0) { direction = Direction::Left;  }
    if (amounts.right > 0) { direction = Direction::Right; }

    return direction;
}

int MovementController::computeMoveAmount(
      Direction dir
    , int baseX, int baseY
    , int deltaX, int deltaY
    , int spriteW, int spriteH
    , int maxMove
    , const Field& field
    ) const {

    // --- 移動量決定ロジック ---
    // 指定方向へ maxMove ピクセル動けるかをチェックし、
    // 壁に当たらない最大の移動量を返す。
    //
    // アルゴリズム:
    //   1. 最大移動量から順に「その距離だけ動いた位置」が衝突しないかを調べる
    //   2. 衝突しない距離が見つかった時点でそれを返す
    //   3. すべて衝突する場合は 0 を返す
    while (maxMove > 0) {
        // maxMoveピクセル進んだ場合の仮位置
        int nextX = baseX + deltaX * maxMove;
        int nextY = baseY + deltaY * maxMove;
        // この位置で衝突しないなら、この量が最大
        if (!m_collisionChecker.isWall(
                  field.getTileSet()
                , field.getTileMap()
                , dir
                , nextX, nextY
                , spriteW, spriteH
                )) {
            return maxMove;  // 移動可能量を返す
        }
        // 衝突する場合は1ピクセル減らして再チェックする
        maxMove--;
    }
    // 1ピクセルも動けない
    return 0;
}
