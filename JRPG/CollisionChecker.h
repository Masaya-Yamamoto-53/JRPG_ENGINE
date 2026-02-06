#pragma once
#include "common.h"
#include "TileSet.h"
#include "TileMap.h"
#include "FieldCharacter.h"

class CollisionChecker {
public:
    // Žw’è•ûŒü‚É•Ç‚ª‚ ‚é‚©‚Ç‚¤‚©‚ð”»’è
    bool isWall(
          const TileSet& tileSet
        , const TileMap& tileMap
        , Direction dir
        , int absCharaX, int absCharaY
        , int spriteW, int spriteH
    ) const;

    bool checkCharacterCollision(
          const FieldCharacter& a
        , const FieldCharacter& b
    ) const {
        return intersects(a.getHitBox(), b.getHitBox());
    }
private:
    static bool intersects(const Rect& a, const Rect& b) {
        return !(a.x + a.w <= b.x
              || b.x + b.w <= a.x
              || a.y + a.h <= b.y
              || b.y + b.h <= a.y
                 );
    }
};
