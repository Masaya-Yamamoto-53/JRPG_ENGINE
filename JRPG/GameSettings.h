#pragma once
#include <string>

class GameSettings {
public:
    // 設定ファイルを読み込む
    bool load(const std::string& filePath);

    // 目標FPSを取得
    int getTargetFps() const { return m_targetFps; }

private:
    int m_targetFps = 60;  // デフォルト値
};
