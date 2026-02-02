#include "DebugManager.h"
#include "DxLib.h"
#include "GameSettings.h"
#include "FpsController.h"

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

void DebugManager::setCharacterPosition(int x, int y, int w, int h) {
    m_charaX = x + w / 2;
    m_charaY = y + h / 2;
}

void DebugManager::draw() const {
    if (!m_enabled) {
        return;
    }
    // デバッグ情報の描画処理をここに実装

    // フォントサイズを設定
    SetFontSize(16);

    int fontSize = GetFontSize();

    // 処理時間[ms], 余裕時間[ms]
    int y = fontSize * 1;

    int white = GetColor(255, 255, 255);
    DrawString(10, y, "FrameTime:", white); y += fontSize;
    DrawFormatString(20, y, white, "Min : %7.3f ms", FpsController::instance().getFrameTimeMin());
    y += fontSize;
    DrawFormatString(20, y, white, "Med : %7.3f ms", FpsController::instance().getFrameTimeMedian());
    y += fontSize;
    DrawFormatString(20, y, white, "Max : %7.3f ms", FpsController::instance().getFrameTimeMax());
    y += fontSize * 2;

    DrawString(10, y, "Slack:", white); y += fontSize;
    DrawFormatString(20, y, white, "Min : %7.3f ms", FpsController::instance().getFrameSlackMin());
    y += fontSize;
    DrawFormatString(20, y, white, "Med : %7.3f ms", FpsController::instance().getFrameSlackMedian());
    y += fontSize;
    DrawFormatString(20, y, white, "Max : %7.3f ms", FpsController::instance().getFrameSlackMax());
    y += fontSize * 2;

    // ウィンドウサイズ
    DrawFormatString(
          10, y, white
        , "WindowSize: w =%4d, h =%4d"
        , GameSettings::instance().getWindowWidth()
        , GameSettings::instance().getWindowHeight()
    );
    y += fontSize;

    // 画面左上にキャラクタの座標を表示する
    DrawFormatString(
          10, y, white
        , "CharaPos:   x =%4d, y =%4d"
        , m_charaX, m_charaY
    );
    y += fontSize;
}
