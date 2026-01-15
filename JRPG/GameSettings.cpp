#include <cstdlib>
#include "DxLib.h"
#include "GameSettings.h"

std::string toAbsolutePath(const std::string& relativePath) {
    char fullPath[_MAX_PATH];
    if (_fullpath(fullPath, relativePath.c_str(), _MAX_PATH) != nullptr) {
        return std::string(fullPath);
    }
    return relativePath; // 失敗したらそのまま返す
}

bool GameSettings::load(const std::string& filePath) {
    // Windows の INI API は相対パスを正しく扱えない場合があるため、絶対パスに変換する
    std::string absPath = toAbsolutePath(filePath);

    // FPS
    int fpsMode = GetPrivateProfileIntA(
          "Graphics"    // セクション名
        , "TargetFps"   // キー名
        , 1             // デフォルトは60FPS
        , absPath.c_str()
    );

    // モード値に応じてFPSを決定
    if (fpsMode == 0) {
        m_targetFps = 30;
    } else {
        m_targetFps = 60;
    }

    // WindowMode
    int windowMode = GetPrivateProfileIntA(
          "Graphics"    // セクション名
        , "WindowMode"  // キー名
        , 1             // デフォルトはウィンドウモード
        , absPath.c_str()
    );

    m_windowMode = windowMode != 0;

    return true;
}