#pragma once
#include "Field.h"
#include "FieldCharacter.h"

class FieldRenderer {
private:
public:
    FieldRenderer();

    void drawField(const Field& field, int counter);
    void drawCharacter(const FieldCharacter& character);
};
