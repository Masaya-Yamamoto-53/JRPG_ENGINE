#pragma once
#include "Field.h"
#include "FieldCharacter.h"

class FieldRenderer {
public:
    FieldRenderer();

    void drawField(const Field& field);
    void drawCharacter(const FieldCharacter& character);
};
