#pragma once
#include "EnemyFactory.h"
#include "EnemyMovementStrategy.h"
#include "EnemyAnimationStrategy.h"

class SimpleEnemyFactory : public EnemyFactory {
public:
    std::unique_ptr<FieldCharacter> create(
        const std::string& type
        , int x, int y
    ) override {

        if (type == "goblin") {
            return std::make_unique<FieldCharacter>(
                  "goblin"
                , "assets\\characters\\enemies\\"
                , std::make_unique<EnemyMovementStrategy>(0, 100, 1)
                , std::make_unique<EnemyAnimationStrategy>()
                , x
                , y
            );
        }

        return nullptr;
    }
};
