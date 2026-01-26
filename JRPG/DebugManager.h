#pragma once
#include "FpsController.h"

class DebugManager {
private:
    bool m_enabled;  // デバッグ表示が有効かどうか
    int m_charaX;    // キャラクタのX座標(中心)
    int m_charaY;    // キャラクタのY座標(中心)

public:
    static DebugManager& instance();

    void toggle();
    bool enabled() const;

    void setCharacterPosition(int x, int y);

    void draw() const;

private:
    DebugManager();
};
