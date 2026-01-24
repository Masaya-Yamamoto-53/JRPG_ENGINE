#pragma once

class DebugManager {
private:
    bool m_enabled;

    int m_charaX;   // キャラクタのX座標(中心)
    int m_charaY;   // キャラクタのY座標(中心)

public:
    static DebugManager& instance();

    void toggle();
    bool enabled() const;

    void setCharacterPosition(int x, int y);

    void draw() const;

private:
    DebugManager();
};
