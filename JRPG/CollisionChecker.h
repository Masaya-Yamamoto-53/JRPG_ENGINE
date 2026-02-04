#pragma once
#include "common.h"
#include "TileSet.h"
#include "TileMap.h"

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
};
