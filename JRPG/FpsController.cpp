#include "DxLib.h"
#include "FpsController.h"

FpsController::FpsController(int targetFps)
    : m_targetFps(targetFps)
    , m_fps(0)
    , m_frameTime(0)
    , m_startTime(GetNowCount())
    , m_frameStart(GetNowCount())
    , m_frameCount(0)
{
}

void FpsController::update() {
    // 現在の時刻を取得
    int now = GetNowCount();

    // 前フレームからの経過時間を計測
    m_frameTime  = now - m_frameStart;
    // 次フレームの基準時刻として更新
    m_frameStart = now;
    // FPS計測用にフレーム数をカウント
    m_frameCount++;
    // 1秒以上経過したらFPSを更新する
    if (now - m_startTime >= 1000) {
        // 1秒間に処理したフレーム数からFPSを産出
        double elapsed = now - m_startTime;
        m_fps = (m_frameCount * 1000.0) / elapsed;

        // 次の1秒計測に向けてリセット
        m_startTime = now;
        m_frameCount = 0;
    }
}

void FpsController::wait() {
    // 1フレームにかけてよい時間
    double frameDuration = 1000.0 / m_targetFps;
    // 現在のフレーム開始からの経過時間
    int frameElapsed  = GetNowCount() - m_frameStart;
    // 目標到達までの待ち時間を計算する
    int waitTime = static_cast<int>(frameDuration - frameElapsed);
    if (waitTime > 0) {
        WaitTimer(waitTime);
    }
}

double FpsController::getFps() const {
    return m_fps;
}