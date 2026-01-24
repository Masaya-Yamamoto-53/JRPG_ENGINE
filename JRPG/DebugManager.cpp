#include "DebugManager.h"
#include "DxLib.h"
#include "GameSettings.h"

DebugManager& DebugManager::instance() {
    static DebugManager instance;
    return instance;
}

DebugManager::DebugManager()
    : m_enabled(false)
{
}

void DebugManager::toggle() {
    m_enabled = !m_enabled;
}

bool DebugManager::enabled() const {
    return m_enabled;
}

void DebugManager::setCharacterPosition(int x, int y) {
    m_charaX = x + GameSettings::instance().getSpriteWidth()  / 2;
    m_charaY = y + GameSettings::instance().getSpriteHeight() / 2;
}

void DebugManager::draw() const {
    if (!m_enabled) {
        return;
    }
    // デバッグ情報の描画処理をここに実装

    // フォントサイズを設定
    SetFontSize(16);

    int fontSize = GetFontSize();

    // ウィンドウサイズ
    DrawFormatString(
          10, fontSize * 1, GetColor(255, 255, 255)
        , "WindowSize: w =%4d, h =%4d"
        , GameSettings::instance().getWindowWidth()
        , GameSettings::instance().getWindowHeight()
    );

    // 画面左上にキャラクタの座標を表示する
    DrawFormatString(
          10, fontSize * 2, GetColor(255, 255, 255)
        , "CharaPos:   x =%4d, y =%4d"
        , m_charaX, m_charaY
    );
}
