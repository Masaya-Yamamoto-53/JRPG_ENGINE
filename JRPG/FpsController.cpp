#include <algorithm>
#include "DxLib.h"
#include "FpsController.h"
#include "DebugManager.h"

FpsController& FpsController::instance() {
    static FpsController instance(60); // デフォルトは60FPS
    return instance;
}

FpsController::FpsController(int targetFps)
    : m_targetFps(targetFps)
    , m_fps(0)
    , m_frameTime(0)
    , m_startTime(GetNowCount())
    , m_frameStart(GetNowCount())
    , m_frameCount(0)
    , m_frameTimeHistoryIndex(0)
    , m_frameTimeHistorySum(0)
    , m_slackTimeHistoryIndex(0)
    , m_slackTimeHistorySum(0)
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&m_qpcFreq);
    QueryPerformanceCounter((LARGE_INTEGER*)&m_qpcStart);

    m_frameTimeHistory.resize(targetFps, 0.0);
    m_slackTimeHistory.resize(targetFps, 0.0);
}

inline double FpsController::getTimeUS() {
    long long now;
    QueryPerformanceCounter((LARGE_INTEGER*)&now);
    return (double)(now - m_qpcStart) * 1000000.0 / m_qpcFreq;
}

void FpsController::update() {
    // 現在の時刻を取得（μs）
    double now = getTimeUS();

    // 前フレームからの経過時間を計測
    m_frameTime  = now - m_frameStart;
    // 次フレームの基準時刻として更新
    m_frameStart = now;
    // FPS計測用にフレーム数をカウント
    m_frameCount++;
    // 1秒以上経過したらFPSを更新する
    if (now - m_startTime >= 1000000.0) {
        // 1秒間に処理したフレーム数からFPSを産出
        double elapsed = now - m_startTime;
        m_fps = (m_frameCount * 1000000.0) / elapsed;

        // 次の1秒計測に向けてリセット
        m_startTime = now;
        m_frameCount = 0;
    }
}

void FpsController::wait() {
    // 1フレームにかけてよい時間
    double frameDuration = 1000000.0 / m_targetFps;
    // 現在のフレーム開始からの経過時間
    double frameElapsed  = getTimeUS() - m_frameStart;
    // 目標到達までの待ち時間を計算する
    double waitTime = frameDuration - frameElapsed;
    if (waitTime > 0) {
        WaitTimer(static_cast<int>(waitTime / 1000.0));  // μs → ms
    }

    if (DebugManager::instance().enabled()) {
        // 履歴更新（リングバッファ）
        m_frameTimeHistorySum -= m_frameTimeHistory[m_frameTimeHistoryIndex];
        m_frameTimeHistory[m_frameTimeHistoryIndex] = frameElapsed;
        m_frameTimeHistorySum += frameElapsed;
        m_frameTimeHistoryIndex = (m_frameTimeHistoryIndex + 1) % m_targetFps;

        m_slackTimeHistorySum -= m_slackTimeHistory[m_slackTimeHistoryIndex];
        m_slackTimeHistory[m_slackTimeHistoryIndex] = waitTime;
        m_slackTimeHistorySum += waitTime;
        m_slackTimeHistoryIndex = (m_slackTimeHistoryIndex + 1) % m_targetFps;
    }

    m_frameStart = getTimeUS();
}

double FpsController::calcMin(const std::vector<double>& data) const {
    double minVal = data[0];
    for (double v : data) {
        if (v < minVal) minVal = v;
    }
    return minVal / 1000.0;
}

double FpsController::calcMax(const std::vector<double>& data) const {
    double maxVal = data[0];
    for (double v : data) {
        if (v > maxVal) maxVal = v;
    }
    return maxVal / 1000.0;
}

double FpsController::calcMedian(std::vector<double> data) const {
    std::sort(data.begin(), data.end());

    int mid = m_targetFps / 2;

    if (m_targetFps % 2 == 1) {
        return data[mid] / 1000.0;
    } else {
        return ((data[mid - 1] + data[mid]) / 2.0) / 1000.0;
    }
}

