#pragma once
#include "Field.h"
#include "FieldCharacter.h"
#include "FieldRenderer.h"

class FieldManager {
private:
    FieldCharacter* m_character;
    FieldRenderer m_renderer;
public:
    FieldManager();

    void setCharacter(FieldCharacter* character);

    void update();
    void draw();
};
