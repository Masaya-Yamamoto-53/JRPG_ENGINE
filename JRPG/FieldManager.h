#pragma once
#include "Field.h"
#include "FieldCharacter.h"
#include "FieldRenderer.h"

class FieldManager {
private:
    Field m_field;
    FieldCharacter* m_character;
    FieldRenderer m_renderer;
public:
    FieldManager();

    void setCharacter(FieldCharacter* character);

    void update();
    void draw();

    void load();
};
