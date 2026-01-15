#pragma once
#include <string>

class GameSettings {
private:
    int m_targetFps = 60;
    bool m_windowMode = true;

public:
    // 設定ファイルを読み込む
    bool load(const std::string& filePath);

    // 目標FPSを取得
    int getTargetFps() const { return m_targetFps; }
    // ウィンドウモードを取得
    bool isWindowMode() const { return m_windowMode; }
};
