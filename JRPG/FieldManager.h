#pragma once
#include <functional>
#include "Field.h"
#include "FieldCharacter.h"
#include "FieldRenderer.h"

class FieldManager {
private:
    Field m_field;
    FieldCharacter* m_character;
    FieldRenderer m_renderer;
    int m_frameCount;
    int m_animationCounter;
public:
    FieldManager();

    void setCharacter(FieldCharacter* character);

    void update();
    void draw();

    void load();
private:
    bool checkMove(
          int& move, bool& flag
        , int baseX, int baseY
        , int deltaX, int deltaY
        , std::function<bool(int, int, int, int)> isWallFunc
    );
};
