#pragma once
#include <memory>
#include <string>
#include "FieldCharacter.h"

class EnemyFactory {
public:
    virtual std::unique_ptr<FieldCharacter> create(
          const std::string& type
        , int x, int y
    ) = 0;

    virtual ~EnemyFactory() = default;
};
