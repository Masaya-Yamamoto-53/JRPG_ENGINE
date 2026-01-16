#include <cstdlib>
#include "DxLib.h"
#include "GameSettings.h"

GameSettings& GameSettings::instance() {
    static GameSettings instance;
    return instance;
}

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

    // WindowMode
    int windowMode = GetPrivateProfileIntA(
          "Graphics"    // セクション名
        , "WindowMode"  // キー名
        , 1             // デフォルトはウィンドウモード
        , absPath.c_str()
    );

    m_windowMode = windowMode != 0;

    // ウィンドウサイズ
    m_windowWidth  = m_fieldTileWidth * m_tileCountX;
    m_windowHeight = m_fieldTileHeight * m_tileCountY;

    return true;
}