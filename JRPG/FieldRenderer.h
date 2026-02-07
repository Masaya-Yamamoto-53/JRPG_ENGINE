#pragma once
#include "Field.h"
#include "FieldCharacter.h"

class FieldRenderer {
private:
public:
    FieldRenderer();

    void renderField(const Field& field);
    void renderCharacter(
          const Field& field
        , const std::vector<std::unique_ptr<FieldCharacter>>& players
        , const std::vector<std::unique_ptr<FieldCharacter>>& enemies
    );
};
